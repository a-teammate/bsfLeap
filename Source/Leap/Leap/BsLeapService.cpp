//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsLeapService.h"
#include "Leap/BsLeapTypes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

namespace bs
{
	static String toString(eLeapRS r)
	{
		switch (r)
		{
		case eLeapRS_Success:
			return "eLeapRS_Success";
		case eLeapRS_UnknownError:
			return "eLeapRS_UnknownError";
		case eLeapRS_InvalidArgument:
			return "eLeapRS_InvalidArgument";
		case eLeapRS_InsufficientResources:
			return "eLeapRS_InsufficientResources";
		case eLeapRS_InsufficientBuffer:
			return "eLeapRS_InsufficientBuffer";
		case eLeapRS_Timeout:
			return "eLeapRS_Timeout";
		case eLeapRS_NotConnected:
			return "eLeapRS_NotConnected";
		case eLeapRS_HandshakeIncomplete:
			return "eLeapRS_HandshakeIncomplete";
		case eLeapRS_BufferSizeOverflow:
			return "eLeapRS_BufferSizeOverflow";
		case eLeapRS_ProtocolError:
			return "eLeapRS_ProtocolError";
		case eLeapRS_InvalidClientID:
			return "eLeapRS_InvalidClientID";
		case eLeapRS_UnexpectedClosed:
			return "eLeapRS_UnexpectedClosed";
		case eLeapRS_UnknownImageFrameRequest:
			return "eLeapRS_UnknownImageFrameRequest";
		case eLeapRS_UnknownTrackingFrameID:
			return "eLeapRS_UnknownTrackingFrameID";
		case eLeapRS_RoutineIsNotSeer:
			return "eLeapRS_RoutineIsNotSeer";
		case eLeapRS_TimestampTooEarly:
			return "eLeapRS_TimestampTooEarly";
		case eLeapRS_ConcurrentPoll:
			return "eLeapRS_ConcurrentPoll";
		case eLeapRS_NotAvailable:
			return "eLeapRS_NotAvailable";
		case eLeapRS_NotStreaming:
			return "eLeapRS_NotStreaming";
		case eLeapRS_CannotOpenDevice:
			return "eLeapRS_CannotOpenDevice";
		default:
			return "unknown result type.";
		}
	}

	LeapService::LeapService()
		: mFrames(_frameBufferLength)
	{
		assert(sizeof(LEAP_VECTOR) == sizeof(Vector3));
		assert(sizeof(LEAP_QUATERNION) == sizeof(Quaternion));
		assert(sizeof(LEAP_BONE) == sizeof(LeapBone));
		assert(sizeof(LEAP_DIGIT) == sizeof(LeapFinger));
		assert(sizeof(LEAP_PALM) == sizeof(LeapPalm));
		assert(sizeof(LEAP_HAND) == sizeof(LeapHand));
		assert(sizeof(LEAP_TRACKING_EVENT) == sizeof(LeapFrame));
	}

	void LeapService::startConnection()
	{
		if (mIsRunning)
			return;

		eLeapRS result;
		if (mConnection == NULL)
		{
			result = LeapCreateConnection(NULL, &mConnection);
			if (result != eLeapRS_Success || mConnection == NULL)
			{
				LOGERR("LeapCreateConnection call was " + toString(result));
				return;
			}
		}
		result = LeapOpenConnection(mConnection);
		if (result != eLeapRS_Success)
		{
			LOGERR("LeapOpenConnection call was " + toString(result));
			return;
		}

		if (mAllocator.allocate == NULL)
			mAllocator.allocate = bs_leap_alloc;

		if (mAllocator.deallocate == NULL)
			mAllocator.deallocate = bs_leap_free;

		LeapSetAllocator(mConnection, &mAllocator);

		mIsRunning = true;
		mThread = bs_new<Thread>(std::bind(&LeapService::processMessageLoop, this));
	}

	void LeapService::stopConnection()
	{
		if (!mIsRunning)
			return;

		mIsRunning = false;

		//Very important to close the connection before we try to join the
		//worker thread!  The call to PollConnection can sometimes block,
		//despite the timeout, causing an attempt to join the thread waiting
		//forever and preventing the connection from stopping.
		//
		//It seems that closing the connection causes PollConnection to 
		//unblock in these cases, so just make sure to close the connection
		//before trying to join the worker thread.
		LeapCloseConnection(mConnection);

		mThread->join();
	}

	void LeapService::destroyConnection()
	{
		stopConnection();
		LeapDestroyConnection(mConnection);
	}

	SPtr<LeapDevice> LeapService::getDeviceActive() const
	{
		auto itFind = std::find_if(mDevices.begin(), mDevices.end(), [&](auto& x) { return x.second->isStreaming(); });
		if (itFind != mDevices.end())
			return itFind->second;

		return NULL;
	}

	SPtr<LeapDevice> LeapService::getDeviceByHandle(LeapDeviceHandle handle) const
	{
		return findDeviceByHandle(handle);
	}

	int64_t LeapService::getNow()
	{
		return LeapGetNow();
	}

	bool LeapService::hasFrame(uint32_t history)
	{
		Lock lock(mMutex);

		return history < mFrames.size();
	}

	const LeapFrame& LeapService::getFrame(uint32_t history)
	{
		Lock lock(mMutex);

		LeapFrame& frame = mFrames[history];
		return frame;
	}

	int64_t LeapService::getFrameTimestamp(uint32_t history)
	{
		Lock lock(mMutex);

		LeapFrame& frame = mFrames[history];
		return frame.mInfo.timestamp;
	}

	bool LeapService::getInterpolatedFrameSize(int64_t timestamp, uint64_t& size)
	{
		eLeapRS result = LeapGetFrameSize(mConnection, timestamp, &size);

		if (result != eLeapRS_Success)
		{
			LOGERR("LeapGetFrameSize call was " + toString(result));
			return false;
		}

		return true;
	}

	bool LeapService::getInterpolatedFrame(int64_t time, LeapFrameAlloc* toFill)
	{
		uint64_t size;
		bool success = getInterpolatedFrameSize(time, size);
		if (!success)
			return false;

		toFill->resize(size);

		LEAP_TRACKING_EVENT* trackingEvent = reinterpret_cast<LEAP_TRACKING_EVENT*>(toFill->get());
		eLeapRS result = LeapInterpolateFrame(mConnection, time, trackingEvent, size);
		if (result != eLeapRS_Success)
		{
			LOGERR("LeapInterpolateFrame call was " + toString(result));
			return false;
		}

		return true;
	}

	bool LeapService::getInterpolatedFrameFromTime(int64_t time, int64_t sourceTime, LeapFrameAlloc* toFill)
	{
		uint64_t size;
		bool success = getInterpolatedFrameSize(time, size);
		if (!success)
			return false;

		toFill->resize(size);

		LEAP_TRACKING_EVENT* trackingEvent = reinterpret_cast<LEAP_TRACKING_EVENT*>(toFill->get());
		eLeapRS result = LeapInterpolateFrameFromTime(mConnection, time, sourceTime, trackingEvent, size);
		if (result != eLeapRS_Success)
		{
			LOGERR("LeapInterpolateFrameFromTime call was " + toString(result));
			return false;
		}

		return true;
	}

	void LeapService::setPolicy(eLeapPolicyFlag policy)
	{
		uint64_t setFlags = (uint64_t)policy;
		mRequestedPolicies = mRequestedPolicies | setFlags;
		setFlags = mRequestedPolicies;
		uint64_t clearFlags = ~mRequestedPolicies; // inverse of desired policies

		eLeapRS result = LeapSetPolicyFlags(mConnection, setFlags, clearFlags);
		if (result != eLeapRS_Success)
			LOGERR("LeapSetPolicyFlags call was " + toString(result));
	}

	void LeapService::clearPolicy(eLeapPolicyFlag policy)
	{
		uint64_t clearFlags = (uint64_t)policy;
		mRequestedPolicies = mRequestedPolicies & ~clearFlags;

		eLeapRS result = LeapSetPolicyFlags(mConnection, mRequestedPolicies, ~mRequestedPolicies);
		if (result != eLeapRS_Success)
			LOGERR("LeapSetPolicyFlagss call was " + toString(result));
	}

	bool LeapService::isConnected() const
	{
		return mIsConnected && mDevices.size() > 0;
	}

	bool LeapService::isServiceConnected() const
	{
		if (mConnection == NULL)
			return false;

		LEAP_CONNECTION_INFO info;

		eLeapRS result = LeapGetConnectionInfo(mConnection, &info);
		if (result != eLeapRS_Success)
			LOGERR("LeapGetConnectionInfo call was " + toString(result));

		if (info.status == eLeapConnectionStatus_Connected)
			return true;

		return false;
	}

	SPtr<LeapDevice> LeapService::findDeviceByHandle(LeapDeviceHandle handle) const
	{
		auto itFind = mDevices.find(handle);
		if (itFind != mDevices.end())
			return itFind->second;

		return NULL;
	}

	void LeapService::pushFrame(const LeapFrame *frame)
	{
		Lock lock(mMutex);
		mFrames.push(*frame);
	}

	void LeapService::processMessageLoop()
	{
		eLeapRS result;
		LEAP_CONNECTION_MESSAGE msg;
		while (mIsRunning)
		{
			unsigned int timeout = 1000;
			result = LeapPollConnection(mConnection, timeout, &msg);

			if (result != eLeapRS_Success)
			{
				LOGWRN("LeapPollConnection call was " + toString(result));
				continue;
			}

			switch (msg.type)
			{
			case eLeapEventType_Connection:
				handleOnConnection(msg.connection_event);
				break;
			case eLeapEventType_ConnectionLost:
				handleOnConnectionLost(msg.connection_lost_event);
				break;
			case eLeapEventType_Device:
				handleOnDevice(msg.device_event);
				break;
			case eLeapEventType_DeviceLost:
				handleOnDeviceLost(msg.device_event);
				break;
			case eLeapEventType_DeviceFailure:
				handleOnDeviceFailure(msg.device_failure_event);
				break;
			case eLeapEventType_Tracking:
				handleOnTracking(msg.tracking_event);
				break;
			case eLeapEventType_ImageComplete:
				// Ignore
				break;
			case eLeapEventType_ImageRequestError:
				// Ignore
				break;
			case eLeapEventType_LogEvent:
				handleOnLog(msg.log_event);
				break;
			case eLeapEventType_LogEvents:
				handleOnLogs(msg.log_events);
				break;
			case eLeapEventType_Policy:
				handleOnPolicy(msg.policy_event);
				break;
			case eLeapEventType_ConfigChange:
				handleOnConfigChange(msg.config_change_event);
				break;
			case eLeapEventType_ConfigResponse:
				handleOnConfigResponse(msg.config_response_event);
				break;
			case eLeapEventType_Image:
				handleOnImage(msg.image_event);
				break;
			case eLeapEventType_PointMappingChange:
				handleOnPointMappingChange(msg.point_mapping_change_event);
				break;
			case eLeapEventType_HeadPose:
				handleOnHeadPose(msg.head_pose_event);
				break;
			default:
				// discard unknown message types
				printf("Unhandled message type %i.\n", msg.type);
			}
		}
	}

	void LeapService::handleOnConnection(const LEAP_CONNECTION_EVENT* connectionEvent)
	{
		mIsConnected = true;
		if (!onConnection.empty())
			onConnection(connectionEvent);
	}

	void LeapService::handleOnConnectionLost(const LEAP_CONNECTION_LOST_EVENT* connectionLostEvent)
	{
		mIsConnected = false;
		if (!onConnectionLost.empty())
			onConnectionLost(connectionLostEvent);
	}

	void LeapService::handleOnDevice(const LEAP_DEVICE_EVENT* deviceEvent)
	{
		// Open device using LEAP_DEVICE_REF from event struct.
		LEAP_DEVICE deviceHandle;
		eLeapRS result = LeapOpenDevice(deviceEvent->device, &deviceHandle);
		if (result != eLeapRS_Success)
		{
			printf("Could not open device %s.\n", toString(result));
			return;
		}

		// Create a struct to hold the device properties.
		LEAP_DEVICE_INFO deviceInfo;
		deviceInfo.serial_length = 0;
		deviceInfo.serial = NULL;
		result = LeapGetDeviceInfo(deviceHandle, &deviceInfo);
		deviceInfo.serial = (char *)malloc(deviceInfo.serial_length);
		result = LeapGetDeviceInfo(deviceHandle, &deviceInfo);
		if (result != eLeapRS_Success)
		{
			printf("Failed to get device info %s.\n", toString(result));
			free(deviceInfo.serial);
			return;
		}

		SPtr<LeapDevice> device = findDeviceByHandle(deviceHandle);

		if (device == NULL)
		{
			device = bs_shared_ptr_new<LeapDevice>();
			mDevices[deviceHandle] = device;
		}

		device->set(deviceHandle, deviceInfo.h_fov, deviceInfo.v_fov, deviceInfo.range / 1000.0f,
			deviceInfo.baseline / 1000.0f, deviceInfo.pid, (deviceInfo.status == eLeapDeviceStatus_Streaming),
			deviceInfo.serial);

		if (!onDevice.empty())
			onDevice(deviceEvent);

		free(deviceInfo.serial);

		LeapCloseDevice(deviceHandle);
	}

	void LeapService::handleOnDeviceLost(const LEAP_DEVICE_EVENT* deviceEvent)
	{
		auto itFind = mDevices.find(deviceEvent->device.handle);
		mDevices.erase(itFind);

		if (!onDeviceLost.empty())
			onDeviceLost(deviceEvent);
	}

	void LeapService::handleOnDeviceFailure(const LEAP_DEVICE_FAILURE_EVENT* deviceFailureEvent)
	{
		String failureMessage;
		String failedSerialNumber = "Unavailable";
		switch (deviceFailureEvent->status)
		{
		case eLeapDeviceStatus_BadCalibration:
			failureMessage = "Bad Calibration. Device failed because of a bad calibration record.";
			break;
		case eLeapDeviceStatus_BadControl:
			failureMessage = "Bad Control Interface. Device failed because of a USB control interface error.";
			break;
		case eLeapDeviceStatus_BadFirmware:
			failureMessage = "Bad Firmware. Device failed because of a firmware error.";
			break;
		case eLeapDeviceStatus_BadTransport:
			failureMessage = "Bad Transport. Device failed because of a USB communication error.";
			break;
		default:
			failureMessage = "Device failed for an unknown reason";
			break;
		}

		auto itFind = mDevices.find(deviceFailureEvent->hDevice);
		if (itFind != mDevices.end())
			mDevices.erase(itFind);

		if (!onDeviceFailure.empty())
			onDeviceFailure(deviceFailureEvent);
	}

	void LeapService::handleOnTracking(const LEAP_TRACKING_EVENT* trackingEvent)
	{
		const LeapFrame* frame = reinterpret_cast<const LeapFrame*>(trackingEvent);
		pushFrame(frame);

		if (!onFrame.empty())
			onFrame(frame);
	}

	void LeapService::handleOnLog(const LEAP_LOG_EVENT* logEvent)
	{
		if (!onLogMessage.empty())
			onLogMessage(logEvent->severity, logEvent->timestamp, logEvent->message);
	}

	void LeapService::handleOnLogs(const LEAP_LOG_EVENTS* logEvents)
	{
		if (!onLogMessage.empty())
		{
			for (int i = 0; i < (int)(logEvents->nEvents); i++)
			{
				const LEAP_LOG_EVENT* logEvent = &logEvents->events[i];
				onLogMessage(logEvent->severity, logEvent->timestamp, logEvent->message);
			}
		}
	}

	void LeapService::handleOnPolicy(const LEAP_POLICY_EVENT* policyEvent)
	{
		if (!onPolicy.empty())
			onPolicy(policyEvent->current_policy);
	}

	void LeapService::handleOnConfigChange(const LEAP_CONFIG_CHANGE_EVENT* configChangeEvent)
	{
		if (!onConfigChange.empty())
			onConfigChange(configChangeEvent->requestID, configChangeEvent->status);
	}

	void LeapService::handleOnConfigResponse(const LEAP_CONFIG_RESPONSE_EVENT* configResponseEvent)
	{
		if (!onConfigResponse.empty())
			onConfigResponse(configResponseEvent->requestID, configResponseEvent->value);
	}

	void LeapService::handleOnImage(const LEAP_IMAGE_EVENT* image_event)
	{
		if (!onImage.empty())
			onImage(image_event);
	}

	void LeapService::handleOnPointMappingChange(const LEAP_POINT_MAPPING_CHANGE_EVENT* pointMappingChangeEvent)
	{
		if (!onPointMappingChange.empty())
			onPointMappingChange(pointMappingChangeEvent);
	}

	void LeapService::handleOnHeadPose(const LEAP_HEAD_POSE_EVENT* headPoseEvent)
	{
		if (!onHeadPose.empty())
			onHeadPose(headPoseEvent);
	}

	void LeapService::onStartUp()
	{
		startConnection();
	}

	void LeapService::onShutDown()
	{
		destroyConnection();
	}

	LeapService &gLeapService()
	{
		return LeapService::instance();
	}
}