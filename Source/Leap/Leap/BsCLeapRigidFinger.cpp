//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "BsCLeapRigidFinger.h"
#include "Components/BsCCapsuleCollider.h"
#include "Components/BsCRigidbody.h"
#include "Private/RTTI/BsCLeapRigidFingerRTTI.h"

namespace bs
{
	CLeapRigidFinger::CLeapRigidFinger()
	{
		setName("LeapRigidFinger");
	}

	CLeapRigidFinger::CLeapRigidFinger(const HSceneObject &parent)
		: CLeapSkeletalFinger(parent)
	{
		setName("LeapRigidFinger");
	}

	void CLeapRigidFinger::onEnabled()
	{
		for (int i = 0; i < NUM_BONES; ++i)
		{
			if (mBones[i] != NULL)
				mBones[i]->getComponent<CRigidbody>()->setMaxAngularVelocity(1e6);
		}
	}

	void CLeapRigidFinger::updateFinger()
	{
		for (int i = 0; i < NUM_BONES; ++i)
		{
			if (mBones[i] != NULL)
			{
				// Set bone dimensions.
				//HCapsuleCollider capsule = mBones[i]->getComponent<CCapsuleCollider>();
				//if (capsule != NULL)
				//{
				//	Vector3 scale = SO()->getTransform().getScale();
				//	// Initialization
				//	//mBones[i]->localScale = new Vector3(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
				//
				//	// Update
				//	capsule->setRadius(getBoneWidth(i) * 0.5f);
				//	capsule->setHalfHeight((getBoneLength(i) + getBoneWidth(i)) * 0.5f);
				//}

				//HRigidbody boneBody = mBones[i]->getComponent<CRigidbody>();
				//if (boneBody)
				//{
				//	boneBody->move(getBoneCenter(i));
				//	//boneBody->rotate(getBoneRotation(i));
				//}
				//else
				{
					//mBones[i]->setWorldPosition(getBoneCenter(i));
					mBones[i]->setWorldPosition(mFinger->mBones[mType].mNextJoint);
					//mBones[i]->setRotation(getBoneRotation(i));
				}
			}
		}
	}

	RTTITypeBase* CLeapRigidFinger::getRTTIStatic()
	{
		return CLeapRigidFingerRTTI::instance();
	}

	RTTITypeBase* CLeapRigidFinger::getRTTI() const
	{
		return CLeapRigidFinger::getRTTIStatic();
	}
}