//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsCLeapSkeletalHand.h"
#include "Private/RTTI/BsCLeapSkeletalHandRTTI.h"

namespace bs
{
	CLeapSkeletalHand::CLeapSkeletalHand()
	{
	}

	CLeapSkeletalHand::CLeapSkeletalHand(const HSceneObject& parent)
		: CLeapHandModel(parent)
	{
	}

	void CLeapSkeletalHand::start()
	{
		for (int i = 0; i < NUM_FINGERS; ++i)
		{
			if (mFingers[i] != NULL)
				mFingers[i]->mType = (LeapFinger::Type)i;
		}
	}

	void CLeapSkeletalHand::updateFrame()
	{
		setPositions();
	}

	Vector3 CLeapSkeletalHand::getPalmCenter()
	{
		Vector3 offset = PALM_CENTER_OFFSET * getPalmDirection() * SO()->getTransform().getScale();
		return getPalmPosition() - offset;
	}

	void CLeapSkeletalHand::setPositions()
	{
		for (int f = 0; f < NUM_FINGERS; ++f)
		{
			if (mFingers[f] != NULL)
				mFingers[f]->update();
		}

		if (mPalm != NULL)
		{
			mPalm->setWorldPosition(getPalmCenter());
			//mPalm->setRotation(getPalmRotation());
		}

		if (mWristJoint != NULL)
		{
			mWristJoint->setWorldPosition(getWristPosition());
			//mWristJoint->setRotation(getPalmRotation());
		}

		if (mForearm != NULL)
		{
			mForearm->setWorldPosition(getArmCenter());
			//mForearm->setRotation(getArmRotation());
		}
	}

	RTTITypeBase* CLeapSkeletalHand::getRTTIStatic()
	{
		return CLeapSkeletalHandRTTI::instance();
	}

	RTTITypeBase* CLeapSkeletalHand::getRTTI() const
	{
		return CLeapSkeletalHand::getRTTIStatic();
	}
}