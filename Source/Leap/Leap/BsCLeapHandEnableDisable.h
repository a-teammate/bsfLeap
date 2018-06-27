//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapPrerequisites.h"
#include "Scene/BsComponent.h"

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	 */

	/**
	 * A component to be attached to a CLeapHandModel to handle enable and disabling
	 * of the SceneObject.
	 */
	class CLeapHandEnableDisable : public Component
	{
	public:
		CLeapHandEnableDisable(const HSceneObject& parent);

	protected:
		/** Called by CLeapHandModelManager when a LeapHand is detected to set the
		* SceneObject active.*/
		void _handReset();

		/** Called by CLeapHandModelManager when a LeapHand is lost to set the
		* SceneObject disabled.*/
		void _handFinish();

	private:
		HLeapHandModelBase mHandModel;

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		/** @copydoc Component::onInitialized */
		void onInitialized() override;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CLeapHandEnableDisableRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CLeapHandEnableDisable();// Serialization only
	};

	/** @} */
}