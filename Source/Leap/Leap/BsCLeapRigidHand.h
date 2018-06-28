//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsCLeapSkeletalHand.h"

namespace bs
{
	/** @addtogroup Leap
	*  @{
	*/

	/** A physics model for a hand made out of various Rigidbody collider. */
	class CLeapRigidHand : public CLeapSkeletalHand
	{
	public:
		CLeapRigidHand(const HSceneObject& parent);

		/** @copydoc CLeapHandModelBase::getKind */
		LeapModelKind getKind() const override { return LeapModelKind::Physics; }

		float filtering = 0.5f;

		bool supportsEditorPersistence() { return true; }

		void updateHand() override;

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CLeapCapsuleHandRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CLeapRigidHand();// Serialization only
	};

	/** @} */
}