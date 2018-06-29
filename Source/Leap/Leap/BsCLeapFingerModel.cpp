//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsCLeapFingerModel.h"
#include "Private/RTTI/BsCLeapFingerModelRTTI.h"

namespace bs
{
	CLeapFingerModel::CLeapFingerModel()
	{
		setName("LeapFingerModel");

		mNotifyFlags = (TransformChangedFlags)(TCF_Parent | TCF_Transform);
	}

	CLeapFingerModel::CLeapFingerModel(const HSceneObject& parent)
		: Component(parent)
	{
		setName("LeapFingerModel");

		mNotifyFlags = (TransformChangedFlags)(TCF_Parent | TCF_Transform);
	}

	void CLeapFingerModel::setLeapHand(const LeapHand* hand)
	{
		mHand = hand;

		if (mHand != NULL)
			mFinger = &mHand->mDigits[(int)mType];
	}

	void CLeapFingerModel::initFinger()
	{
		updateFinger();
	}

	Vector3 CLeapFingerModel::getTipPosition()
	{
		if (mFinger != NULL)
		{
			Vector3 localTip = mFinger->mBones[LeapBone::TYPE_DISTAL].mNextJoint;
			return localTip;
		}
		if (mBones[NUM_BONES - 1] && mJoints[NUM_JOINTS - 2])
		{
			return 2.0f * mBones[NUM_BONES - 1]->getTransform().getPosition() -
				mJoints[NUM_JOINTS - 2]->getTransform().getPosition();
		}
		return Vector3::ZERO;
	}

	Vector3 CLeapFingerModel::getJointPosition(int joint)
	{
		if (joint >= NUM_BONES)
		{
			return getTipPosition();
		}
		if (mFinger != NULL)
		{
			Vector3 localPosition = mFinger->mBones[joint].mPrevJoint;
			return localPosition;
		}
		if (mJoints[joint])
		{
			return mJoints[joint]->getTransform().getPosition();
		}
		return Vector3::ZERO;
	}

	Ray CLeapFingerModel::getRay()
	{
		Ray ray(getTipPosition(), getBoneDirection(NUM_BONES - 1));
		return ray;
	}

	Vector3 CLeapFingerModel::getBoneCenter(int bone_type)
	{
		if (mFinger != NULL)
		{
			const LeapBone& bone = mFinger->mBones[bone_type];
			return (bone.mNextJoint + bone.mPrevJoint) * 0.5f;
		}
		if (mBones[bone_type])
		{
			return mBones[bone_type]->getTransform().getPosition();
		}
		return Vector3::ZERO;
	}

	Vector3 CLeapFingerModel::getBoneDirection(int bone_type)
	{
		if (mFinger != NULL)
		{
			Vector3 direction = getJointPosition(bone_type + 1) - getJointPosition(bone_type);
			direction.normalize();
			return direction;
		}
		if (mBones[bone_type])
		{
			return mBones[bone_type]->getTransform().getForward();
		}
		return -Vector3::UNIT_Z;
	}

	/** Returns the rotation quaternion of the given bone */

	Quaternion CLeapFingerModel::getBoneRotation(int bone_type)
	{
		if (mFinger != NULL)
		{
			Quaternion localRotation = mFinger->mBones[bone_type].mRotation;
			return localRotation;
		}
		if (mBones[bone_type])
		{
			return mBones[bone_type]->getTransform().getRotation();
		}
		return Quaternion::IDENTITY;
	}

	float CLeapFingerModel::getBoneLength(int bone_type)
	{
		const LeapBone& bone = mFinger->mBones[bone_type];
		return (bone.mNextJoint - bone.mPrevJoint).length();
	}

	float CLeapFingerModel::getFingerJointStretchMecanim(int joint_type)
	{
		// The successive actions of local rotations on a vector yield the global rotation,
		// so the inverse of the parent rotation appears on the left.
		Quaternion jointRotation = Quaternion::IDENTITY;
		if (mFinger != NULL)
		{
			jointRotation = mFinger->mBones[joint_type].mRotation.inverse()
				* mFinger->mBones[joint_type + 1].mRotation;
		}
		else if (mBones[joint_type] && mBones[joint_type + 1])
		{
			jointRotation = getBoneRotation(joint_type).inverse() * getBoneRotation(joint_type + 1);
		}
		// Stretch is a rotation around the X axis of the base bone
		// Positive stretch opens joints
		Radian xAngle, yAngle, zAngle;
		jointRotation.toEulerAngles(xAngle, yAngle, zAngle);
		float stretchAngle = -xAngle.valueDegrees();
		if (stretchAngle <= -180.0f)
		{
			stretchAngle += 360.0f;
		}
		// NOTE: eulerAngles range is [0, 360) so stretchAngle > +180f will not occur.
		return stretchAngle;
	}

	float CLeapFingerModel::getFingerJointSpreadMecanim()
	{
		// The successive actions of local rotations on a vector yield the global rotation,
		// so the inverse of the parent rotation appears on the left.
		Quaternion jointRotation = Quaternion::IDENTITY;
		if (mFinger != NULL)
		{
			jointRotation = mFinger->mBones[0].mRotation.inverse()
				* mFinger->mBones[(LeapBone::Type)(1)].mRotation;
		}
		else if (mBones[0] && mBones[1])
		{
			jointRotation = getBoneRotation(0).inverse() * getBoneRotation(1);
		}
		// Spread is a rotation around the Y axis of the base bone when joint_type = 0
		float spreadAngle = 0.0f;
		LeapFinger::Type fType = mType;
		//if (mFinger != NULL)
		//{
		//	mType = mFinger->mType;
		//}

		if (fType == LeapFinger::TYPE_INDEX || fType == LeapFinger::TYPE_MIDDLE)
		{
			Radian xAngle, yAngle, zAngle;
			jointRotation.toEulerAngles(xAngle, yAngle, zAngle);
			spreadAngle = yAngle.valueDegrees();
			if (spreadAngle > 180.0f)
			{
				spreadAngle -= 360.0f;
			}
			// NOTE: eulerAngles range is [0, 360) so spreadAngle <= -180f will not occur.
		}
		if (fType == LeapFinger::TYPE_THUMB || fType == LeapFinger::TYPE_RING || fType == LeapFinger::TYPE_PINKY)
		{
			Radian xAngle, yAngle, zAngle;
			jointRotation.toEulerAngles(xAngle, yAngle, zAngle);
			spreadAngle = -yAngle.valueDegrees();
			if (spreadAngle <= -180.0f)
			{
				spreadAngle += 360.0f;
			}
			// NOTE: eulerAngles range is [0, 360) so spreadAngle > +180f will not occur.
		}
		return spreadAngle;
	}

	RTTITypeBase* CLeapFingerModel::getRTTIStatic()
	{
		return CLeapFingerModelRTTI::instance();
	}

	RTTITypeBase* CLeapFingerModel::getRTTI() const
	{
		return CLeapFingerModel::getRTTIStatic();
	}
}