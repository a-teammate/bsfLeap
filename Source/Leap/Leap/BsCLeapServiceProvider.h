//************************************ bs::framework - Copyright 2018 Next Limit **************************************** *//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ********** *//
#pragma once

#include "Leap/BsLeapService.h"
#include "Scene/BsComponent.h"
#include "Utility/BsSmoothedFloat.h"

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	  */

	/**
	 * The HandModelManager manages a pool of LeapHandModelBases and makes
	 * HandRepresentations when it detects a Leap Hand from LeapService.
	 *
	 * When a LeapHandRepresentation is created, a HandModelBase is removed from the
	 * pool. When a HandRepresentation is finished, its HandModelBase is returned to
	 * the pool.
	 */
	class CLeapServiceProvider : public Component
	{
	public:
		enum FrameOptimizationMode {
			FrameOptimizationNone,
			FrameOptimizationReuseUpdateForPhysics,
			FrameOptimizationReusePhysicsForUpdate,
		};

		enum PhysicsExtrapolationMode {
			PhysicsExtrapolationNone,
			PhysicsExtrapolationAuto,
			PhysicsExtrapolationManual
		};

	public:
		CLeapServiceProvider(const HSceneObject &parent);

		/**
		 * The current frame for this update cycle, in world space.
		 *
		 * IMPORTANT!  This frame might be mutable!  If you hold onto a reference
		 * to this frame, or a reference to any object that is a part of this frame,
		 * it might change unexpectedly.  If you want to save a reference, make sure
		 * to make a copy.
		 */
		LeapFrame* getCurrentFrame();

		/**
		 * The current frame for this fixed update cycle, in world space.
		 *
		 * IMPORTANT!  This frame might be mutable!  If you hold onto a reference
		 * to this frame, or a reference to any object that is a part of this frame,
		 * it might change unexpectedly.  If you want to save a reference, make sure
		 * to make a copy.
		 */
		LeapFrame* getCurrentFixedFrame();

		/**
		 * Returns true if the Leap Motion hardware is plugged in and this application is
		 * connected to the Leap Motion service.
		 */
		bool isConnected();

		/**
		 * Retransforms hand data from Leap space to the space of the Unity transform.
		 * This is only necessary if you're moving the LeapServiceProvider around in a
		 * custom script and trying to access Hand data from it directly afterward.
		 */
		void retransformFrames();

	public:
		typedef void(*PfnOnDevice)(SPtr<LeapDevice> device);

		/** Event to get a callback whenever a new device is connected to the service. */
		Event<void(SPtr<LeapDevice> device)> onDeviceSafe;

		Event<void(const LeapFrame*)> onUpdateFrame;
		Event<void(const LeapFrame*)> onFixedFrame;

		/**
		* Event to get a callback whenever a new device is connected to the service.
		* This callback will ALSO trigger a callback upon subscription if a device is connected.
		*/
		HEvent onDeviceSafeConnect(std::function<void(SPtr<LeapDevice>)> func);

	protected:
		/**
		* Initializes leap Motion policy flags and subscribes to its connection event.
		*/
		void initializeService();

		/**
		 * Initializes Leap Motion policy flags.
		 */
		virtual void initializeFlags();

		/**
		 * Stops the connection for the existing service.
		 */
		void releaseService();

		/**
		 * Checks whether the provider is connected to a service. If it is not, attempt to
		 * reconnect at regular intervals for MAX_RECONNECTION_ATTEMPTS.
		 */
		bool checkConnectionIntegrity();

		int64_t calculateInterpolationTime();

		float calculatePhysicsExtrapolation();

		void handleUpdateFrameEvent(LeapFrame* frame);

		void handleFixedFrameEvent(LeapFrame* frame);

		void _transformFrame(const LeapFrameAlloc& source, LeapFrameAlloc& dest);

	private:
		void onDeviceInit(const LEAP_DEVICE_EVENT *device_event);

		void triggerOnDeviceSafe(const LEAP_DEVICE_EVENT *device_event);

	protected:
		/** The maximum number of times the provider will attempt to reconnect to the service. */
		const int MAX_RECONNECTION_ATTEMPTS = 5;

		/** The number of frames to wait between each reconnection attempt. */
		const int RECONNECTION_INTERVAL = 180;

		/** When enabled, the provider will only calculate one leap frame instead of two. */
		FrameOptimizationMode mFrameOptimization = FrameOptimizationNone;

		/**
		 * The mode to use when extrapolating physics.
		 * None - No extrapolation is used at all.
		 * Auto - Extrapolation is chosen based on the fixed timestep.
		 * Manual - Extrapolation time is chosen manually by the user.
		 */
		PhysicsExtrapolationMode mPhysicsExtrapolation = PhysicsExtrapolationAuto;

		bool mUseInterpolation = true;

		int mBounceAmount = 0;

		int mExtrapolationAmount = 0;

		/** The amount of time (in seconds) to extrapolate the physics data by. */
		float mPhysicsExtrapolationTime = 1.0f / 90.0f;

		LeapService* mLeap = NULL;

		SmoothedFloat mSmoothedTrackingLatency;
		uint64_t mBsToLeapOffset;

		LeapFrameAlloc mUntransformedUpdateFrame;
		LeapFrameAlloc mTransformedUpdateFrame;
		LeapFrameAlloc mUntransformedFixedFrame;
		LeapFrameAlloc mTransformedFixedFrame;

	private:
		int mFramesSinceServiceConnectionChecked = 0;
		int mNumberOfReconnectionAttempts = 0;

		HEvent mOnDeviceInitConn;

		/*********************************************************************** */
		/* 						COMPONENT OVERRIDES                      		 */
		/*********************************************************************** */
	public:
		/** @copydoc Component::update  */
		void update() override;

		/** @copydoc Component::fixedUpdate  */
		void fixedUpdate() override;

	protected:
		friend class SceneObject;

		/** @copydoc Component::onInitialized  */
		void onInitialized() override;

		/** @copydoc Component::onDestroyed */
		void onDestroyed() override;

		/** @copydoc Component::onDisabled  */
		void onDisabled() override;

		/** @copydoc Component::onEnabled  */
		void onEnabled() override;

		/*********************************************************************** */
		/* 								RTTI		                     		 */
		/*********************************************************************** */
	public:
		friend class CLeapServiceProviderRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CLeapServiceProvider();// Serialization only
	};

	/** @}  */
}