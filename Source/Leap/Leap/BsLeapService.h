//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapDevice.h"
#include "Leap/BsLeapFrameAlloc.h"
#include "Leap/BsLeapTypes.h"
#include "Utility/BsCircularBuffer.h"
#include "Utility/BsEvent.h"
#include "Utility/BsModule.h"

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	 */

	/**
	 * The LeapService module is your main interface to the Leap Motion Controller.
	 * 
	 * Create an instance of this Controller class to access frames of tracking
	 * data and configuration information.Frame data can be polled at any time
	 * using the Controller.Frame() function.Call frame() or frame(0) to get the
	 * most recent frame.Set the history parameter to a positive integer to access
	 * previous frames.A controller stores up to 60 frames in its frame history.
	 * 
	 * 
	 * Polling is an appropriate strategy for applications which already have an
	 * intrinsic update loop, such as a game. You can also subscribe to the FrameReady
	 * event to get tracking frames through an event delegate.
	 * 
	 * If the current thread implements a SynchronizationContext that contains a message
	 * loop, events are posted to that threads message loop. Otherwise, events are called
	 * on an independent thread and applications must perform any needed synchronization
	 * or marshalling of data between threads. Note that Unity3D does not create an
	 * appropriate SynchronizationContext object. Typically, event handlers cannot access
	 * any Unity objects.
	 */
	class LeapService : public Module<LeapService>
	{
	public:
		/** Leap service constructor. */
		LeapService();

		/**
		 * Creates and opens a connection to the Leap Motion service.
		 * On success, creates a thread to service the LeapC message pump.
		 */
		void startConnection();

		/** Closes a previously opened connection. */
		void stopConnection();

		/** Destroys a previously opened connection. */
		void destroyConnection();

		/**
		* The list of currently attached and recognized Leap Motion controller devices.
		*/
		const Map<void*, LeapDevice*>& getDevices() const { return mDevices; }

		/** The device that is currently streaming tracking data. */
		LeapDevice* getDeviceActive();

		/** Returns the device with provided handle. */
		LeapDevice* getDeviceByHandle(void* handle);

		int64_t getNow();

		/**
		 * Returns if the service has a frame. Use the optional history parameter to specify 
		 * which frame to retrieve.
		 */
		bool hasFrame(uint32_t history = 0);

		/**
		 * In most cases you should get Frame objects using the LeapProvider.CurrentFrame
		 * property. The data in Frame objects taken directly from a Leap.Controller instance
		 * is still in the Leap Motion frame of reference and will not match the hands
		 * displayed in a Unity scene.
		 * 
		 * Returns a frame of tracking data from the Leap Motion software. Use the optional
		 * history parameter to specify which frame to retrieve. Call frame() or
		 * frame(0) to access the most recent frame; call frame(1) to access the
		 * previous frame, and so on. If you use a history value greater than the
		 * number of stored frames, then the controller returns an empty frame.
		 * 
		 * @param history The age of the frame to return, counting backwards from
		 * the most recent frame (0) into the past and up to the maximum age (59).
		 * @returns The specified frame; or, if no history parameter is specified,
		 * the newest frame. If a frame is not available at the specified history
		 * position, an invalid Frame is returned.
		 */
		LeapFrame& getFrame(uint32_t history = 0);

		/**
		 * Returns the timestamp of a recent tracking frame.  Use the optional history parameter
		 * to specify how many frames in the past to retrieve the timestamp.  Leave the history
		 * parameter as it's default value to return the timestamp of the most recent frame.
		 */
		int64_t getFrameTimestamp(uint32_t history = 0);

		/**
		 * Retrieves the number of bytes required to allocate an interpolated frame at the specified 
		 * time.
		 * @param timestamp The timestamp of the frame whose size is to be queried.
		 * @param[out] size Receives the number of bytes required to store the specified frame.
		 * @returns true if frame interompoation is possible, false otherwise.
		 */
		bool getInterpolatedFrameSize(int64_t timestamp, uint64_t& size);

		/**
		 * Returns the frame at the specified time, interpolating the data between existing
		 * frames, if necessary.
		 */
		bool getInterpolatedFrame(int64_t timestamp, LeapFrameAlloc* toFill);

		bool getInterpolatedFrameFromTime(int64_t time, int64_t sourceTime, LeapFrameAlloc* toFill);

		void setPolicy(eLeapPolicyFlag policy);
		void clearPolicy(eLeapPolicyFlag policy);

		/**
		 * Reports whether has a connection to the Leap Motion daemon/service.
		 * Can be true even if the Leap Motion hardware is not available.
		 */
		bool isServiceConnected() const;

		/**
		 * Reports whether this Controller is connected to the Leap Motion service and
		 * the Leap Motion hardware is plugged in.
		 * 
		 * When you first create the LeapService, isConnected() returns false.
		 * After the service finishes initializing and connects to the Leap Motion
		 * software and if the Leap Motion hardware is plugged in, isConnected() returns true.
		*/
		bool isConnected() const;

	public:
		typedef void(*PfnConnection)(const LEAP_CONNECTION_EVENT* connection_event);
		typedef void(*PfnConnectionLost)(const LEAP_CONNECTION_LOST_EVENT *connection_lost_event);
		typedef void(*PfnDevice)(const LEAP_DEVICE_INFO *device);
		typedef void(*PfnDeviceLost)();
		typedef void(*PfnDeviceFailure)(const LEAP_DEVICE_FAILURE_EVENT *device_failure_event);
		typedef void(*PfnPolicy)(const uint32_t current_policies);
		typedef void(*PfnTracking)(const LeapFrame *tracking_event);
		typedef void(*PfnLog)(const eLeapLogSeverity severity, const int64_t timestamp, const char *message);
		typedef void(*PfnConfigChange)(const uint32_t requestID, const bool success);
		typedef void(*PfnConfigResponse)(const uint32_t requestID, LEAP_VARIANT value);
		typedef void(*PfnImage)(const LEAP_IMAGE_EVENT *evt);
		typedef void(*PfnPointMappingChange)(const LEAP_POINT_MAPPING_CHANGE_EVENT *evt);
		typedef void(*PfnHeadPose)(const LEAP_HEAD_POSE_EVENT *evt);

		Event<void(const LEAP_CONNECTION_EVENT* connection_event)> onConnection;
		Event<void(const LEAP_CONNECTION_LOST_EVENT *connection_lost_event)> onConnectionLost;
		Event<void(const LEAP_DEVICE_EVENT *device_event)> onDevice;
		Event<void(const LEAP_DEVICE_EVENT *device_event)> onDeviceLost;
		Event<void(const LEAP_DEVICE_FAILURE_EVENT *device_failure_event)> onDeviceFailure;
		Event<void(const uint32_t current_policies)> onPolicy;
		Event<void(const LeapFrame *tracking_event)> onFrame;
		Event<void(const eLeapLogSeverity severity, const int64_t timestamp, const char *message)> onLogMessage;
		Event<void(const uint32_t requestID, const bool success)> onConfigChange;
		Event<void(const uint32_t requestID, LEAP_VARIANT value)> onConfigResponse;
		Event<void(const LEAP_IMAGE_EVENT *evt)> onImage;
		Event<void(const LEAP_POINT_MAPPING_CHANGE_EVENT *evt)> onPointMappingChange;
		Event<void(const LEAP_HEAD_POSE_EVENT *evt)> onHeadPose;

	private:
		/**
		* Services the LeapC message pump by calling LeapPollConnection().
		* The average polling time is determined by the framerate of the Leap Motion service.
		*/
		void processMessageLoop();

		/** For internal use only. */
		LeapDevice* createDevice();

		/** For internal use only. */
		void destroyDevice(LeapDevice* device);

		/** For internal use only. */
		LeapDevice* findDeviceByHandle(void* handle);

		/** Caches the newest frame by copying the tracking event struct returned by LeapC. */
		void pushFrame(const LeapFrame *frame);

		void handleOnConnection(const LEAP_CONNECTION_EVENT* connection_event);

		void handleOnConnectionLost(const LEAP_CONNECTION_LOST_EVENT *connection_lost_event);

		void handleOnDevice(const LEAP_DEVICE_EVENT *device_event);

		void handleOnDeviceLost(const LEAP_DEVICE_EVENT *device_event);

		void handleOnDeviceFailure(const LEAP_DEVICE_FAILURE_EVENT *device_failure_event);

		void handleOnTracking(const LEAP_TRACKING_EVENT *tracking_event);

		void handleOnLog(const LEAP_LOG_EVENT *log_event);

		void handleOnLogs(const LEAP_LOG_EVENTS *log_events);

		void handleOnPolicy(const LEAP_POLICY_EVENT *policy_event);

		void handleOnConfigChange(const LEAP_CONFIG_CHANGE_EVENT *config_change_event);

		void handleOnConfigResponse(const LEAP_CONFIG_RESPONSE_EVENT *config_response_event);

		void handleOnImage(const LEAP_IMAGE_EVENT *image_event);

		void handleOnPointMappingChange(const LEAP_POINT_MAPPING_CHANGE_EVENT *point_mapping_change_event);

		void handleOnHeadPose(const LEAP_HEAD_POSE_EVENT *head_pose_event);

	private:
		LEAP_CONNECTION mConnection = NULL;
		bool mIsConnected = false;

		LEAP_ALLOCATOR mAllocator;

		Mutex mMutex;
		Thread* mThread;
		volatile bool mIsRunning = false;

		static constexpr int32_t _frameBufferLength = 60;

		CircularBuffer<LeapFrame> mFrames;

		Map<void*, LeapDevice*> mDevices;

		//Policy and enabled features
		uint64_t mRequestedPolicies = 0;
		uint64_t mActivePolicies = 0;

		/*********************************************************************** */
		/* 							MODULE OVERRIDES                      		 */
		/*********************************************************************** */
	public:
		/** @copydoc Module::onStartUp */
		void onStartUp() override;

		/** @copydoc Module::onShutDown */
		void onShutDown() override;
	};

	/** Provides easier access to LeapService. */
	LeapService &gLeapService();

	/** @} */
}