//************************************ bs::framework - Copyright 2018 Next Limit **************************************** *//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ********** *//
#pragma once

#include "Leap/BsLeapPrerequisites.h"

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	 */

	/**
	 * The LeapDevice class represents a physically connected device.
	 * 
	 * The LeapDevice class contains information related to a particular connected
	 * device such as device id, field of view relative to the device,
	 * and the position and orientation of the device in relative coordinates.
	 * 
	 * The position and orientation describe the alignment of the device relative to the user.
	 * The alignment relative to the user is only descriptive. Aligning devices to users
	 * provides consistency in the parameters that describe user interactions.
	 * 
	 * Note that LeapDevice objects can be invalid, which means that they do not contain
	 * valid device information and do not correspond to a physical device.
	 * @since 1.0
	 */
	class LeapDevice
	{
	public:
		/** Leap device constructor. */
		void set(void* deviceHandle,
				float horizontalViewAngle,
				float verticalViewAngle,
				float range,
				float baseline,
				eLeapDevicePID type,
				bool isStreaming,
				String serialNumber)
		{
			mHandle = deviceHandle;
			mHorizontalViewAngle = horizontalViewAngle;
			mVerticalViewAngle = verticalViewAngle;
			mRange = range;
			mBaseline = baseline;
			mType = type;
			mIsStreaming = isStreaming;
			mSerialNumber = serialNumber;
		}

		/**
		 * For internal use only.
		 */
		void* getHandle() const { return mHandle; }

		/**
		 * The angle in radians of view along the x axis of this device.
		 * 
		 * The Leap Motion controller scans a region in the shape of an inverted pyramid
		 * centered at the device's center and extending upwards. The horizontalViewAngle
		 * reports the view angle along the long dimension of the device.
		 */
		float getHorizontalViewAngle() const { return mHorizontalViewAngle; }

		/**
		 * The angle in radians of view along the z axis of this device.
		 * 
		 * The Leap Motion controller scans a region in the shape of an inverted pyramid
		 * centered at the device's center and extending upwards. The verticalViewAngle
		 * reports the view angle along the short dimension of the device.
		 */
		float getVerticalViewAngle() const { return mVerticalViewAngle; }

		/**
		 * The maximum reliable tracking range from the center of this device.
		 * 
		 * The range reports the maximum recommended distance from the device center
		 * for which tracking is expected to be reliable. This distance is not a hard limit.
		 * Tracking may be still be functional above this distance or begin to degrade slightly
		 * before this distance depending on calibration and extreme environmental conditions.
		 */
		float getRange() const { return mRange; }

		/**
		 * The distance in mm between the center points of the stereo sensors.
		 * 
		 * The baseline value, together with the maximum resolution, influence the
		 * maximum range.
		 */
		float getBaseline() const { return mBaseline; }

		/**
		 * Reports whether this device is streaming data to your application.
		 * 
		 * Currently only one controller can provide data at a time.
		 */
		bool isStreaming() const { return mIsStreaming; }

		/**
		 * The device type.
		 * 
		 * Use the device type value in the (rare) circumstances that you
		 * have an application feature which relies on a particular type of device.
		 * Current types of device include the original Leap Motion peripheral,
		 * keyboard-embedded controllers, and laptop-embedded controllers.
		 */
		eLeapDevicePID getType() const { return mType; }

		/**
		 * An alphanumeric serial number unique to each device.
		 * 
		 * Consumer device serial numbers consist of 2 letters followed by 11 digits.
		 * 
		 * When using multiple devices, the serial number provides an unambiguous
		 * identifier for each device.
		 */
		String getSerialNumber() const { return mSerialNumber; }

		/**
		 * Compare LeapDevice object equality.
		 *
		 * Two LeapDevice objects are equal if and only if both LeapDevice objects represent the
		 * exact same LeapDevice and both Devices are valid.
		 */
		friend bool operator==(const LeapDevice& l, const LeapDevice& r)
		{
			return l.mSerialNumber == r.mSerialNumber;
		}

		/**
		 * A string containing a brief, human readable description of the LeapDevice object.
		 */
		String toString()
		{
			return "LeapDevice serial# " + mSerialNumber;
		}

	private:
		/** Handle to a Leap device object. */
		void* mHandle;

		/** The angle in radians of view along the x axis of this device. */
		float mHorizontalViewAngle;

		/** The angle in radians of view along the z axis of this device. */
		float mVerticalViewAngle;

		/** The maximum reliable tracking range from the center of this device. */
		float mRange;

		/** The distance in mm between the center points of the stereo sensors. */
		float mBaseline;

		/** Reports whether this device is streaming data to your application. */
		bool mIsStreaming;

		/** The device type. */
		eLeapDevicePID mType;

		/** An alphanumeric serial number unique to each device. */
		String mSerialNumber;
	};

	/** @} */
}