//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsCLeapHandModel.h"

namespace bs
{
	/** @addtogroup Leap
	*  @{
	*/

	/**
	* A hand object consisting of discrete, component parts.
	*
	* The hand can have scene objects for the palm, wrist and forearm, as well as fingers.
	*/
	class CLeapSkeletalHand : public CLeapHandModel
	{
	public:
		CLeapSkeletalHand(const HSceneObject& parent);

	protected:
		const float PALM_CENTER_OFFSET = 0.015f;


	public:
		/** Updates the hand and its component parts by setting their positions and rotations. */
		void updateHand() override;

	protected:
		Vector3 getPalmCenter();

		void setPositions();

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		void start();

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class ComponentRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CLeapSkeletalHand(); // Serialization only
	};

	/** @} */
}