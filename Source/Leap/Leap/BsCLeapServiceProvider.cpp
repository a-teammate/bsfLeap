//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsCLeapServiceProvider.h"
#include "Leap/BsLeapTypes.h"
#include "Leap/BsLeapService.h"
#include "Private/RTTI/BsCLeapServiceProviderRTTI.h"
#include "BsCoreApplication.h"
#include "Utility/BsTime.h"

using namespace std::placeholders;

/** Converts nanoseconds to seconds. */
static constexpr double NS_TO_S = 1e-6;

/** Converts seconds to nanoseconds. */
static constexpr double S_TO_NS = 1e6;

namespace bs
{
	CLeapServiceProvider::CLeapServiceProvider()
	{
		setName("LeapServiceProvider");
		setFlag(ComponentFlag::AlwaysRun, true);
	}

	CLeapServiceProvider::CLeapServiceProvider(const HSceneObject &parent)
		: Component(parent)
	{
		setName("LeapServiceProvider");
		setFlag(ComponentFlag::AlwaysRun, true);
	}

	LeapFrame* CLeapServiceProvider::getCurrentFrame()
	{
		if (mFrameOptimization == FrameOptimizationReusePhysicsForUpdate) {
			return mTransformedFixedFrame.get();
		}
		else {
			return mTransformedUpdateFrame.get();
		}
	}

	LeapFrame* CLeapServiceProvider::getCurrentFixedFrame()
	{
		if (mFrameOptimization == FrameOptimizationReuseUpdateForPhysics) {
			return mTransformedUpdateFrame.get();
		}
		else {
			return mTransformedFixedFrame.get();
		}
	}

	bool CLeapServiceProvider::isConnected()
	{
		return mLeap->isConnected();
	}

	float CLeapServiceProvider::calculatePhysicsExtrapolation()
	{
		switch (mPhysicsExtrapolation)
		{
		case PhysicsExtrapolationNone:
			return 0;
		case PhysicsExtrapolationAuto:
			return gCoreApplication().getFixedUpdateStep();
		case PhysicsExtrapolationManual:
			return mPhysicsExtrapolationTime;
		default:
			LOGERR("Unexpected physics extrapolation mode: " + mPhysicsExtrapolation);
		}
	}

	void CLeapServiceProvider::retransformFrames()
	{
		transformFrame(mUntransformedUpdateFrame.get(), mTransformedUpdateFrame.get());
		transformFrame(mUntransformedFixedFrame.get(), mTransformedFixedFrame.get());
	}

	HEvent CLeapServiceProvider::onDeviceSafeConnect(std::function<void(const LeapDevice*)> func)
	{
		if (mLeap->isConnected()) {
			for (auto& it : mLeap->getDevices()) {
				func(it.second);
			}
		}

		return onDeviceSafe.connect(func);
	}

	void CLeapServiceProvider::initializeService()
	{
		if (mLeap != NULL) {
			return;
		}

		mLeap = &gLeapService();

		// trigger onDeviceSafe
		mOnDeviceInitConn = mLeap->onDevice.connect(
			std::bind(&CLeapServiceProvider::triggerOnDeviceSafe, this, _1));

		if (mLeap->isConnected()) {
			initializeFlags();
		}
		else {
			mOnDeviceInitConn = mLeap->onDevice.connect(
				std::bind(&CLeapServiceProvider::onDeviceInit, this, _1));
		}
	}

	void CLeapServiceProvider::initializeFlags()
	{
		if (mLeap == NULL) {
			return;
		}

		mLeap->clearPolicy((eLeapPolicyFlag)0);
	}

	void CLeapServiceProvider::releaseService()
	{
		if (mLeap == NULL) {
			return;
		}

		if (mLeap->isConnected()) {
			mLeap->clearPolicy(eLeapPolicyFlag_OptimizeHMD);
		}

		mLeap->stopConnection();
		mLeap = NULL;
	}

	bool CLeapServiceProvider::checkConnectionIntegrity()
	{
		if (mLeap->isServiceConnected()) {
			mFramesSinceServiceConnectionChecked = 0;
			mNumberOfReconnectionAttempts = 0;
			return true;
		}
		else if (mNumberOfReconnectionAttempts < MAX_RECONNECTION_ATTEMPTS) {
			mFramesSinceServiceConnectionChecked++;

			if (mFramesSinceServiceConnectionChecked > RECONNECTION_INTERVAL) {
				mFramesSinceServiceConnectionChecked = 0;
				mNumberOfReconnectionAttempts++;

				String msg = "Leap Service not connected; attempting to reconnect for try " +
					toString(mNumberOfReconnectionAttempts) + "/" + toString(MAX_RECONNECTION_ATTEMPTS) + "...";
				LOGWRN(msg);
				releaseService();
				initializeService();
			}
		}
		return false;
	}

	void CLeapServiceProvider::handleUpdateFrameEvent(LeapFrame* frame)
	{
		if (!onUpdateFrame.empty()) {
			onUpdateFrame(frame);
		}
	}

	void CLeapServiceProvider::handleFixedFrameEvent(LeapFrame* frame)
	{
		if (!onFixedFrame.empty()) {
			onFixedFrame(frame);
		}
	}

	int64_t CLeapServiceProvider::calculateInterpolationTime()
	{
		if (mLeap != NULL) {
			return mLeap->getNow() - (uint64_t)mSmoothedTrackingLatency.mValue;
		}
		else {
			return 0;
		}
	}

	void CLeapServiceProvider::transformFrame(const LeapFrame* source, LeapFrame* dest)
	{
		const Transform& transform = SO()->getTransform();
		//dest->CopyFrom(source).Transform(transform.GetLeapMatrix());
	}

	void CLeapServiceProvider::onDeviceInit(const LEAP_DEVICE_EVENT *device_event)
	{
		initializeFlags();

		mOnDeviceInitConn.disconnect();
	}

	void CLeapServiceProvider::triggerOnDeviceSafe(const LEAP_DEVICE_EVENT *device_event)
	{
		if (!onDeviceSafe.empty()) {
			LeapDevice *device = mLeap->getDeviceByHandle(device_event->device.handle);
			onDeviceSafe(device);
		}
	}

	void CLeapServiceProvider::update()
	{
		if (!mLeap->isConnected() || !mLeap->hasFrame()) {
			checkConnectionIntegrity();
			return;
		}

		if (mFrameOptimization == FrameOptimizationReusePhysicsForUpdate) {
			handleUpdateFrameEvent(mTransformedFixedFrame.get());
			return;
		}

		if (mUseInterpolation) {
			float trackingLatency = (float)(mLeap->getNow() - mLeap->getFrameTimestamp());
			mSmoothedTrackingLatency.mValue = std::min(mSmoothedTrackingLatency.mValue, 30000.0f);
			mSmoothedTrackingLatency.update(trackingLatency, gTime().getFrameDelta());

			int64_t interpolationTime = calculateInterpolationTime();
			int64_t timestamp = interpolationTime + (mExtrapolationAmount * 1000);
			int64_t sourceTimestamp = interpolationTime - (mBounceAmount * 1000);

			bool success = mLeap->getInterpolatedFrameFromTime(timestamp, sourceTimestamp,
				&mUntransformedUpdateFrame);
			if (!success) {
				return;
			}

			mBsToLeapOffset = timestamp - (uint64_t)(gTime().getTime() * S_TO_NS);
		}
		else {
			*mUntransformedUpdateFrame.get() = mLeap->getFrame();
		}

		if (mUntransformedUpdateFrame.get() != NULL) {
			transformFrame(mUntransformedUpdateFrame.get(), mTransformedUpdateFrame.get());

			//handleUpdateFrameEvent(mTransformedUpdateFrame.get());
			handleUpdateFrameEvent(mUntransformedUpdateFrame.get());
		}
	}

	void CLeapServiceProvider::fixedUpdate()
	{
		if (!mLeap->isConnected() || !mLeap->hasFrame()) {
			return;
		}

		if (mFrameOptimization == FrameOptimizationReuseUpdateForPhysics) {
			handleFixedFrameEvent(mTransformedUpdateFrame.get());
			return;
		}

		if (mUseInterpolation) {

			uint64_t timestamp;
			switch (mFrameOptimization) {
			case FrameOptimizationNone: {
				// By default we use gCoreApplication().getFixedUpdateStep() to ensure that our hands are on the same
				// timeline as Update.  We add an extrapolation value to help compensate
				// for latency.
				float extrapolatedTime = gCoreApplication().getLastFixedUpdateTime() + calculatePhysicsExtrapolation();
				timestamp = (uint64_t)(extrapolatedTime * S_TO_NS) + mBsToLeapOffset;
			} break;
			case FrameOptimizationReusePhysicsForUpdate: {
				// If we are re-using physics frames for update, we don't even want to care
				// about gCoreApplication().getFixedUpdateStep(), just grab the most recent interpolated timestamp
				// like we are in Update.
				timestamp = calculateInterpolationTime() + (mExtrapolationAmount * 1000);
			} break;
			default:
				LOGERR("Unexpected frame optimization mode: " + mFrameOptimization);
			}
			bool success = mLeap->getInterpolatedFrame(timestamp, &mUntransformedFixedFrame);
			if (!success) {
				return;
			}
		}
		else {
			*mUntransformedFixedFrame.get() = mLeap->getFrame();
		}

		if (mUntransformedFixedFrame.get() != NULL) {
			transformFrame(mUntransformedFixedFrame.get(), mTransformedFixedFrame.get());

			handleFixedFrameEvent(mTransformedFixedFrame.get());
		}
	}

	void CLeapServiceProvider::onInitialized()
	{
		mSmoothedTrackingLatency.setBlend(0.99f, 0.0111f);
	}

	void CLeapServiceProvider::onDestroyed()
	{
		releaseService();
	}

	void CLeapServiceProvider::onDisabled()
	{

	}

	void CLeapServiceProvider::onEnabled()
	{
		initializeService();
	}

	RTTITypeBase* CLeapServiceProvider::getRTTIStatic()
	{
		return CLeapServiceProviderRTTI::instance();
	}

	RTTITypeBase* CLeapServiceProvider::getRTTI() const
	{
		return CLeapServiceProvider::getRTTIStatic();
	}
}