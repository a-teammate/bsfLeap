//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsCLeapSkeletalFinger.h"
#include "Private/RTTI/BsCLeapSkeletalFingerRTTI.h"

namespace bs
{
	CLeapSkeletalFinger::CLeapSkeletalFinger()
	{
	}

	CLeapSkeletalFinger::CLeapSkeletalFinger(const HSceneObject& parent)
		: CLeapFingerModel(parent)
	{
	}

	void CLeapSkeletalFinger::onInitModel()
	{
		setPositions();
	}

	void CLeapSkeletalFinger::updateFrame()
	{
		setPositions();
	}

	void CLeapSkeletalFinger::setPositions()
	{
		for (int i = 0; i < NUM_BONES; ++i)
		{
			if (mBones[i] != NULL)
			{
				mBones[i]->setWorldPosition(getBoneCenter(i));
				//mBones[i]->setRotation(getBoneRotation(i));
			}
		}

		for (int i = 0; i < NUM_JOINTS; ++i)
		{
			if (mJoints[i] != NULL)
			{
				mJoints[i]->setWorldPosition(getJointPosition(i + 1));
				//mJoints[i]->setRotation(getBoneRotation(i + 1));
			}
		}
	}

	RTTITypeBase* CLeapSkeletalFinger::getRTTIStatic()
	{
		return CLeapSkeletalFingerRTTI::instance();
	}

	RTTITypeBase* CLeapSkeletalFinger::getRTTI() const
	{
		return CLeapSkeletalFinger::getRTTIStatic();
	}
}