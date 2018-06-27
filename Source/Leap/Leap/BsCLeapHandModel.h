//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsCLeapHandModelBase.h"
#include "Scene/BsTransform.h"

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	 */

	/**
	 * The base class for all hand models, both graphics and physics.
	 *
	 * This class serves as the interface between the HandController object
	 * and the concrete hand object containing the graphics and physics of a hand.
	 *
	 * Subclasses of HandModel must implement InitHand() and UpdateHand(). The
	 * UpdateHand() function is called in the Unity Update() phase for graphics
	 * HandModel instances; and in the Unity FixedUpdate() phase for physics
	 * objects. InitHand() is called once, when the hand is created and is followed
	 * by a call to UpdateHand().
	 */
	class CLeapHandModel : public CLeapHandModelBase
	{
	public:
		/** The number of fingers on a hand.*/
		const int NUM_FINGERS = 5;

		eLeapHandType getChirality() const override { return mChirality; }

		LeapModelType getType() const override { return mType; }

		/** Calculates the position of the palm in global coordinates.
		* @returns A Vector3 containing the Unity coordinates of the palm position.
		*/
		Vector3 getPalmPosition() { return mHand->mPalm.mPosition; }

		/** Calculates the rotation of the hand in global coordinates.
		* @returns A Quaternion representing the rotation of the hand.
		*/
		Quaternion getPalmRotation() {
			if (mHand != NULL) {
				// The hand Basis vectors are calculated explicitly.  This requires using
				// Basis.CalculateRotation() instead of Basis.quaternion.
				//return mHand->getTransform().getRotation();
				return mHand->mPalm.mOrientation;
			}
			if (mPalm) {
				return mPalm->getRotation();
			}
			return Quaternion::IDENTITY;
		}

		/** Calculates the direction vector of the hand in global coordinates.
		* @returns A Vector3 representing the direction of the hand.
		*/
		Vector3 getPalmDirection() {
			if (mHand != NULL) {
				return mHand->mPalm.mDirection;
			}
			if (mPalm) {
				return mPalm->getForward();
			}
			return Vector3::UNIT_Z;
		}

		/** Calculates the normal vector projecting from the hand in global
		* coordinates.
		* @returns A Vector3 representing the vector perpendicular to the palm.
		*/
		Vector3 getPalmNormal() {
			if (mHand != NULL) {
				return mHand->mPalm.mNormal;
			}
			if (mPalm) {
				return -mPalm->getUp();
			}
			return -Vector3::UNIT_Y;
		}

		/** Calculates the direction vector of the forearm in global coordinates.
		* @returns A Vector3 representing the direction of the forearm (pointing from
		* elbow to wrist).
		*/
		Vector3 getArmDirection() {
			if (mHand != NULL) {
				Vector3 direction = mHand->mArm.mNextJoint - mHand->mArm.mPrevJoint;
				direction.normalize();
				return direction;
			}
			if (mForearm) {
				return mForearm->getForward();
			}
			return Vector3::UNIT_Z;
		}

		/** Calculates the center of the forearm in global coordinates.
		* @returns A Vector3 containing the Unity coordinates of the center of the
		* forearm.
		*/
		Vector3 getArmCenter() {
			if (mHand != NULL) {
				Vector3 center = 0.5f * (mHand->mArm.mPrevJoint + mHand->mArm.mNextJoint);
				return center;
			}
			if (mForearm) {
				return mForearm->getPosition();
			}
			return Vector3::ZERO;
		}

		/** Returns the measured length of the forearm in meters.*/
		float getArmLength() {
			return (mHand->mArm.mNextJoint - mHand->mArm.mPrevJoint).length();
		}

		/** Returns the measured width of the forearm in meters.*/
		float getArmWidth() { return mHand->mArm.mWidth; }

		/** Calculates the position of the elbow in global coordinates.
		* @returns A Vector3 containing the Unity coordinates of the elbow.
		*/
		Vector3 getElbowPosition() {
			if (mHand != NULL) {
				Vector3 local_position = mHand->mArm.mPrevJoint;
				return local_position;
			}
			if (mElbowJoint) {
				return mElbowJoint->getPosition();
			}
			return Vector3::ZERO;
		}

		/** Calculates the position of the wrist in global coordinates.
		* @returns A Vector3 containing the Unity coordinates of the wrist.
		*/
		Vector3 getWristPosition() {
			if (mHand != NULL) {
				Vector3 local_position = mHand->mArm.mNextJoint;
				return local_position;
			}
			if (mWristJoint) {
				return mWristJoint->getPosition();
			}
			return Vector3::ZERO;
		}

		/** Calculates the rotation of the forearm in global coordinates.
		* @returns A Quaternion representing the rotation of the arm.
		*/
		Quaternion getArmRotation() {
			if (mHand != NULL) {
				Quaternion local_rotation = mHand->mArm.mRotation;
				return local_rotation;
			}
			if (mForearm) {
				return mForearm->getRotation();
			}
			return Quaternion::IDENTITY;
		}

		LeapHand *getLeapHand() const override { return mHand; }

		void setLeapHand(LeapHand *hand) override {
			mHand = hand;
			for (int i = 0; i < NUM_FINGERS; ++i) {
				// if (mFingers[i] != NULL) {
				//  mFingers[i].SetLeapHand(mHand);
				//}
			}
		}

		void init() override {
			for (int f = 0; f < NUM_FINGERS; ++f) {
				// if (mFingers[f] != NULL) {
				//  mFingers[f].fingerType = (Finger.FingerType)f;
				//  mFingers[f].InitFinger();
				//}
			}
		}

		/**
		* Returns the ID associated with the hand in the Leap API.
		* This ID is guaranteed to be unique among all hands in a frame,
		* and is invariant for the lifetime of the hand model.
		*/
		int getLeapID() {
			if (mHand != NULL) {
				return mHand->mId;
			}
			return -1;
		}

	public:
		eLeapHandType mChirality;

		/** The model width of the hand in meters. This value is used with the
		* measured value of the user's hand to scale the model proportionally.
		*/
		float handModelPalmWidth = 0.085f;
		/** The array of finger objects for this hand. The array is ordered from thumb
		* (element 0) to pinky (element 4).*/
		// LeapFingerModel[] mFingers = new LeapFingerModel[NUM_FINGERS];

		/** Transform object for the palm object of this hand. */
		Transform *mPalm;
		/** Transform object for the forearm object of this hand. */
		Transform *mForearm;
		/** Transform object for the wrist joint of this hand. */
		Transform *mWristJoint;
		/** Transform object for the elbow joint of this hand. */
		Transform *mElbowJoint;

	protected:
		/** The Leap Hand object this hand model represents. */
		LeapHand *mHand;

	private:
		LeapModelType mType;

		/************************************************************************/
		/* 								SERIALIZATION                      		*/
		/************************************************************************/
	private:
		CLeapHandModel(); // Serialization only

	public:
		friend class CLeapHandModelRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;
	};

	/** @} */
}