//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsCLeapSkeletalFinger.h"

namespace bs
{
	/** @addtogroup Leap
	*  @{
	*/

	/** A physics finger model for our rigid hand made out of various cube Unity Colliders. */
	class CLeapRigidFinger : public CLeapSkeletalFinger
	{
	public:
		CLeapRigidFinger(const HSceneObject &parent);

		void updateFinger() override;

		float filtering = 0.5f;

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		void onEnabled() override;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CLeapCapsuleHandRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CLeapRigidFinger();// Serialization only
	};

	/** @} */
}