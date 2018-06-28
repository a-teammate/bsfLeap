//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsCLeapHandModel.h"
#include "Private/RTTI/BsCLeapHandModelRTTI.h"

namespace bs
{
	CLeapHandModel::CLeapHandModel()
	{
		setName("LeapHandModel");

		mNotifyFlags = (TransformChangedFlags)(TCF_Parent | TCF_Transform);
	}

	CLeapHandModel::CLeapHandModel(const HSceneObject& parent)
		: CLeapHandModelBase(parent)
	{
		setName("LeapHandModel");

		mNotifyFlags = (TransformChangedFlags)(TCF_Parent | TCF_Transform);
	}

	Quaternion CLeapHandModel::getPalmRotation()
	{
		if (mHand != NULL)
		{
			// The hand Basis vectors are calculated explicitly.  This requires using
			// Basis.CalculateRotation() instead of Basis.quaternion.
			//return mHand->getTransform().getRotation();
			return mHand->mPalm.mOrientation;
		}
		if (mPalm)
		{
			return mPalm->getTransform().getRotation();
		}
		return Quaternion::IDENTITY;
	}

	Vector3 CLeapHandModel::getPalmDirection()
	{
		if (mHand != NULL)
			return mHand->mPalm.mDirection;
		if (mPalm)
			return mPalm->getTransform().getForward();
		return Vector3::UNIT_Z;
	}

	Vector3 CLeapHandModel::getPalmNormal()
	{
		if (mHand != NULL)
			return mHand->mPalm.mNormal;
		if (mPalm)
			return -mPalm->getTransform().getUp();
		return -Vector3::UNIT_Y;
	}

	Vector3 CLeapHandModel::getArmDirection()
	{
		if (mHand != NULL)
		{
			Vector3 direction = mHand->mArm.mNextJoint - mHand->mArm.mPrevJoint;
			direction.normalize();
			return direction;
		}
		if (mForearm)
		{
			return mForearm->getTransform().getForward();
		}
		return Vector3::UNIT_Z;
	}

	Vector3 CLeapHandModel::getArmCenter()
	{
		if (mHand != NULL)
		{
			Vector3 center = 0.5f * (mHand->mArm.mPrevJoint + mHand->mArm.mNextJoint);
			return center;
		}
		if (mForearm)
		{
			return mForearm->getTransform().getPosition();
		}
		return Vector3::ZERO;
	}

	Vector3 CLeapHandModel::getElbowPosition()
	{
		if (mHand != NULL)
		{
			Vector3 local_position = mHand->mArm.mPrevJoint;
			return local_position;
		}
		if (mElbowJoint)
			return mElbowJoint->getTransform().getPosition();
		return Vector3::ZERO;
	}

	Vector3 CLeapHandModel::getWristPosition()
	{
		if (mHand != NULL)
		{
			Vector3 local_position = mHand->mArm.mNextJoint;
			return local_position;
		}
		if (mWristJoint)
			return mWristJoint->getTransform().getPosition();
		return Vector3::ZERO;
	}

	Quaternion CLeapHandModel::getArmRotation()
	{
		if (mHand != NULL)
		{
			Quaternion local_rotation = mHand->mArm.mRotation;
			return local_rotation;
		}
		if (mForearm)
			return mForearm->getTransform().getRotation();
		return Quaternion::IDENTITY;
	}

	void CLeapHandModel::setLeapHand(const LeapHand* hand)
	{
		mHand = hand;
		for (int i = 0; i < NUM_FINGERS; ++i)
		{
			// if (mFingers[i] != NULL) {
			//  mFingers[i].SetLeapHand(mHand);
			//}
		}
	}

	void CLeapHandModel::initHand()
	{
		for (int f = 0; f < NUM_FINGERS; ++f)
		{
			// if (mFingers[f] != NULL) {
			//  mFingers[f].fingerType = (Finger.FingerType)f;
			//  mFingers[f].InitFinger();
			//}
		}
	}

	int CLeapHandModel::getLeapID()
	{
		if (mHand != NULL)
		{
			return mHand->mId;
		}
		return -1;
	}

	RTTITypeBase* CLeapHandModel::getRTTIStatic()
	{
		return CLeapHandModelRTTI::instance();
	}

	RTTITypeBase* CLeapHandModel::getRTTI() const
	{
		return CLeapHandModel::getRTTIStatic();
	}
}