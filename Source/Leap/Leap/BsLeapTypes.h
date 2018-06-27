//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Math/BsVector3.h"
#include "Math/BsQuaternion.h"
#include "Leap/BsLeapPrerequisites.h"

#pragma pack(1)

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	 */

	/**
	 * Describes a bone's position and orientation.
	 */
	struct LeapBone
	{
		/** The base of the bone, closer to the heart. The bones origin. */
		Vector3 mPrevJoint;

		/** The end of the bone, further from the heart. */

		Vector3 mNextJoint;

		/** The average width of the flesh around the bone in millimeters. */
		float mWidth;

		/** Rotation in world space from the forward direction. */
		Quaternion mRotation;
	};

	/**
	 * Describes the digit of a hand.
	 */
	struct LeapFinger
	{
		/** The Leap identifier of this finger. */
		INT32 mFingerId;

		union {
			/** All the bones of a digit as an iterable collection. */
			LeapBone mBones[4];

			struct {
				/**
				* The finger bone wholly inside the hand.
				* For thumbs, this bone is set to have zero length and width, an identity basis matrix,
				* and its joint positions are equal.
				*/
				LeapBone mMetacarpal;

				/** The phalange extending from the knuckle. */
				LeapBone mProximal;

				/** The bone between the proximal phalange and the distal phalange. */
				LeapBone mIntermediate;

				/** The distal phalange terminating at the finger tip. */
				LeapBone mDistal;
			};
		};

		UINT32 mIsExtended;
	};

	/**
	 * Properties associated with the palm of the hand.
	 */
	struct LeapPalm
	{
		/** The center position of the palm in millimeters from the Leap Motion origin. */
		Vector3 mPosition;

		/**
		* The time-filtered and stabilized position of the palm.
		*
		* Smoothing and stabilization is performed in order to make
		* this value more suitable for interaction with 2D content. The stabilized
		* position lags behind the palm position by a variable amount, depending
		* primarily on the speed of movement.
		*/
		Vector3 mStabilizedPosition;

		/** The rate of change of the palm position in millimeters per second. */
		Vector3 mVelocity;

		/**
		* The normal vector to the palm. If your hand is flat, this vector will
		* point downward, or "out" of the front surface of your palm.
		*/
		Vector3 mNormal;

		/** The estimated width of the palm when the hand is in a flat position. */
		float mWidth;

		/** The unit direction vector pointing from the palm position toward the fingers. */
		Vector3 mDirection;

		/**
		* The quaternion representing the palm's orientation
		* corresponding to the basis {normal x direction, -normal, -direction}
		*/
		Quaternion mOrientation;
	};

	/**
	 * Describes a tracked hand.
	 */
	typedef struct LeapHand
	{
		/**
		* A unique ID for a hand tracked across frames.
		* If tracking of a physical hand is lost, a new ID is assigned when tracking is reacquired.
		*/
		UINT32 mId;

		/** Reserved for future use. */
		UINT32 mFlags;

		/** Identifies the chirality of this hand. */
		eLeapHandType mType;

		/** How confident we are with a given hand pose. Not currently used (always 1.0). */
		float mConfidence;

		/** The total amount of time this hand has been tracked, in microseconds. */
		UINT64 mVisibleTime;

		/** The distance between index finger and thumb. */
		float mPinchDistance;

		/** The average angle of fingers to palm. */
		float mGrabAngle;

		/**
		 * The normalized estimate of the pinch pose.
		 * Zero is not pinching; one is fully pinched.
		 */
		float mPinchStrength;

		/**
		* The normalized estimate of the grab hand pose.
		* Zero is not grabbing; one is fully grabbing.
		*/
		float mGrabStrength;

		/** Additional information associated with the palm. */
		LeapPalm mPalm;

		/** The fingers of this hand. */
		union {
			struct {
				/** The thumb. */
				LeapFinger mThumb;
				/** The index finger. */
				LeapFinger mIndex;
				/** The middle finger. */
				LeapFinger mMiddle;
				/** The ring finger. */
				LeapFinger mRing;
				/** The pinky finger. */
				LeapFinger mPinky;
			};
			/** The fingers of the hand as an array. */
			LeapFinger mDigits[5];
		};

		/**
		* The arm to which this hand is attached.
		* An arm consists of a single LeapBone struct.
		*/
		LeapBone mArm;
	};

	/**
	* A snapshot, or frame of data, containing the tracking data for a single moment in time.
	* The LeapFrame struct is the container for all the tracking data.
	*/
	struct LeapFrame
	{
		/** A universal frame identification header. */
		LEAP_FRAME_HEADER mInfo;

		/**
		* An identifier for this tracking frame. This identifier is meant to be monotonically
		* increasing, but values may be skipped if the client application does not poll for messages
		* fast enough. This number also generally increases at the same rate as info.frame_id, but
		* if the server cannot process every image received from the device cameras, the info.frame_id
		* identifier may increase faster.
		*/
		INT64 mTrackingFrameId;

		/** The number of hands tracked in this frame. */
		UINT32 mNumberOfHands;

		/** A pointer to the array of hands tracked in this frame. */
		LeapHand* mHands;

		/**
		* Current tracking frame rate in hertz.
		*
		* This frame rate is distinct from the image frame rate, which is the rate that images are
		* being read from the device. Depending on host CPU limitations, the tracking frame rate
		* may be substantially less than the device frame rate.
		*
		* This number is generally equal to or less than the device frame rate, but there is one
		* case where this number may be _higher_ than the device frame rate:  When the device rate
		* drops. In this case, the device frame rate will fall sooner than the tracking frame rate.
		*
		* This number is equal to zero if there are not enough frames to estimate frame rate.
		*
		* This number cannot be negative.
		*/
		float mFramerate;
	};

	/** @} */
}

#pragma pack()
