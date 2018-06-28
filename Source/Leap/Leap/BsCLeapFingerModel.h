//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Math/BsRay.h"
#include "Leap/BsLeapTypes.h"
#include "Scene/BsSceneObject.h"

namespace bs
{
	/** @addtogroup Leap
	*  @{
	*/

	/**
	 * The base class for all fingers.
	 *
	 * This class serves as the interface between the LeapServiceProvider, the parent LeapHand and the finger objects.
	 *
	 * Subclasses of CLeapFingerModel must implement init() and update(). The InitHand() function is typically called by
	 * the parent HandModel InitHand() method; likewise, the UpdateFinger() function is typically called by the parent
	 * HandModel UpdateHand() function.
	 */
	class CLeapFingerModel : public Component
	{
	public:
		CLeapFingerModel(const HSceneObject& parent);

		/** Returns the LeapHand object. */
		const LeapHand* getLeapHand() const { return mHand; }

		/** Returns the LeapFinger object. */
		const LeapFinger* getLeapFinger() const { return mFinger; }

		/** Sets the Leap Hand and Leap Finger for this finger. */
		void setLeapHand(const LeapHand* hand);

		/**
		* Implement this function to initialize this finger after it is created.
		* Typically, this function is called by the parent LeapHandModel object.
		*/
		virtual void initFinger();

		/**
		* Implement this function to update this finger.
		* Typically, this function is called by the parent CLeapHandModel object's updateHand() function.
		*/
		virtual void updateFinger() = 0;

		/** Returns the location of the tip of the finger */
		Vector3 getTipPosition();

		/** Returns the location of the given joint on the finger */
		Vector3 getJointPosition(int joint);

		/** Returns a ray from the tip of the finger in the direction it is pointing.*/
		Ray getRay();

		/** Returns the center of the given bone on the finger */
		Vector3 getBoneCenter(int bone_type);

		/** Returns the direction the given bone is facing on the finger */
		Vector3 getBoneDirection(int bone_type);

		/** Returns the rotation quaternion of the given bone */
		Quaternion getBoneRotation(int bone_type);

		/** Returns the length of the finger bone.*/
		float getBoneLength(int bone_type);

		/** Returns the width of the finger bone.*/
		float getBoneWidth(int bone_type)
		{
			return mFinger->mBones[bone_type].mWidth;
		}

		/**
		* Returns Mecanim stretch angle in the range (-180, +180]
		* NOTE: Positive stretch opens the hand.
		* For the thumb this moves it away from the palm.
		*/
		float getFingerJointStretchMecanim(int joint_type);

		/**
		* Returns Mecanim spread angle, which only applies to joint_type = 0
		* Positive spread is towards thumb for index and middle, but is in the opposite direction for the ring and pinky.
		* For the thumb negative spread rotates the thumb in to the palm.
		* */
		float getFingerJointSpreadMecanim();

	public:
		/** The number of bones in a finger. */
		static constexpr int NUM_BONES = 4;

		/** The number of joints in a finger. */
		static constexpr int NUM_JOINTS = 3;

		LeapFinger::Type mType = LeapFinger::TYPE_INDEX;

		/** Bones positioned and rotated by FingerModel. */
		HSceneObject mBones[NUM_BONES];

		/** Joints positioned and rotated by FingerModel. */
		HSceneObject mJoints[NUM_BONES - 1];

	protected:
		/** LeapHand object. */
		const LeapHand* mHand;

		/** LeapFinger object. */
		const LeapFinger* mFinger;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CLeapFingerModelRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CLeapFingerModel();// Serialization only
	};

	/** @} */
}