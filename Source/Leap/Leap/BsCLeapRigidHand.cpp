//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "BsCLeapRigidHand.h"
#include "Components/BsCCapsuleCollider.h"
#include "Components/BsCRigidbody.h"
#include "Private/RTTI/BsCLeapRigidHandRTTI.h"

namespace bs
{
	CLeapRigidHand::CLeapRigidHand()
	{
		setName("LeapRigidHand");
	}

	CLeapRigidHand::CLeapRigidHand(const HSceneObject &parent)
		: CLeapSkeletalHand(parent)
	{
		setName("LeapRigidHand");
	}

	void CLeapRigidHand::updateFrame()
	{
		for (int f = 0; f < NUM_FINGERS; ++f)
		{
			if (mFingers[f] != NULL)
				mFingers[f]->updateFrame();
		}

		if (mPalm != NULL)
		{
			HRigidbody palmBody = mPalm->getComponent<CRigidbody>();
			if (palmBody)
			{
				palmBody->move(getPalmCenter());
				//palmBody->rotate(getPalmRotation());
			}
			else
			{
				mPalm->setWorldPosition(getPalmCenter());
				//mPalm->setRotation(getPalmRotation());
			}
		}

		if (mForearm != NULL)
		{
			// Set arm dimensions.
			HCapsuleCollider capsule = mForearm->getComponent<CCapsuleCollider>();
			if (capsule != NULL)
			{
				Vector3 scale = SO()->getTransform().getScale();
				// Initialization
				//mForearm->localScale = new Vector3(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);

				// Update
				capsule->setRadius(getArmWidth() * 0.5f);
				capsule->setHalfHeight((getArmLength() + getArmWidth()) * 0.5f);
			}

			HRigidbody forearmBody = mForearm->getComponent<CRigidbody>();
			if (forearmBody)
			{
				forearmBody->move(getArmCenter());
				//forearmBody->rotate(getArmRotation());
			}
			else
			{
				mForearm->setWorldPosition(getArmCenter());
				//mForearm->setRotation(getArmRotation());
			}
		}
	}

	RTTITypeBase* CLeapRigidHand::getRTTIStatic()
	{
		return CLeapRigidHandRTTI::instance();
	}

	RTTITypeBase* CLeapRigidHand::getRTTI() const
	{
		return CLeapRigidHand::getRTTIStatic();
	}
}