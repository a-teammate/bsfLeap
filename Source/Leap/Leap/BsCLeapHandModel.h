//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsCLeapFingerModel.h"
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
	 * Subclasses of HandModel must implement init() and update(). update() function is called in the update() phase
	 * for graphics LeapHandModel instances; and in the fixedUpdate() phase for physics objects. init() is called once,
	 * when the hand is created and is followed by a call to UpdateHand().
	 */
	class CLeapHandModel : public CLeapHandModelBase
	{
	public:
		CLeapHandModel(const HSceneObject& parent);

		/** @copydoc CLeapHandModelBase::getChirality */
		eLeapHandType getChirality() const override { return mChirality; }

		/** @copydoc CLeapHandModelBase::getKind */
		LeapModelKind getKind() const override { return mKind; }

		/** Returns the position of the palm in global coordinates. */
		Vector3 getPalmPosition() { return mHand->mPalm.mPosition; }

		/** Returns the rotation of the hand in global coordinates. */
		Quaternion getPalmRotation();

		/** Returns the direction vector of the hand in global coordinates. */
		Vector3 getPalmDirection();

		/** Returns the normal vector projecting from the hand in global coordinates. */
		Vector3 getPalmNormal();

		/** Returns the direction vector of the forearm in global coordinates. */
		Vector3 getArmDirection();

		/** Returns the center of the forearm in global coordinates. */
		Vector3 getArmCenter();

		/** Returns the measured length of the forearm in meters.*/
		float getArmLength();

		/** Returns the measured width of the forearm in meters.*/
		float getArmWidth() { return mHand->mArm.mWidth; }

		/** Returns the position of the elbow in global coordinates. */
		Vector3 getElbowPosition();

		/** Returns the position of the wrist in global coordinates. */
		Vector3 getWristPosition();

		/** Returns the rotation of the forearm in global coordinates. */
		Quaternion getArmRotation();

		const LeapHand* getLeapHand() const override { return mHand; }

		void setLeapHand(const LeapHand* hand) override;

		void initHand() override;

		/**
		* Returns the ID associated with the hand in the Leap API. This ID is guaranteed to be unique among all hands
		* in a frame, and is invariant for the lifetime of the hand model.
		*/
		int getLeapID();

	public:
		/** The number of fingers on a hand.*/
		static constexpr int NUM_FINGERS = 5;

		eLeapHandType mChirality;

		/**
		 * The model width of the hand in meters. This value is used with the measured value of the user's hand to scale
		 * the model proportionally.
		 */
		float handModelPalmWidth = 0.085f;

		/** The array of finger objects for this hand. The array is ordered from thumb (element 0) to pinky (element 4). */
		HLeapFingerModel mFingers[NUM_FINGERS];

		/** Scene object for the palm of this hand. */
		HSceneObject mPalm;
		/** Scene object for the forearm of this hand. */
		HSceneObject mForearm;
		/** Scene object for the wrist joint of this hand. */
		HSceneObject mWristJoint;
		/** Scene object for the elbow joint of this hand. */
		HSceneObject mElbowJoint;

	protected:
		/** The LeapHand object this hand model represents. */
		const LeapHand* mHand = NULL;

	private:
		LeapModelKind mKind;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CLeapHandModelRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CLeapHandModel(); // Serialization only
	};

	/** @} */
}