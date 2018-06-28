//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsCLeapFingerModel.h"

namespace bs
{
	/** @addtogroup Leap
	*  @{
	*/

	/**
	 * A finger object consisting of discrete, component parts for each bone.
	 *
	 * The graphic objects can include both bones and joints, but both are optional.
	 */
	class CLeapSkeletalFinger : public CLeapFingerModel
	{
	public:
		CLeapSkeletalFinger(const HSceneObject& parent);

		/** Initializes the finger bones and joints by setting their positions and rotations. */
		void initFinger() override;

		/** Updates the finger bones and joints by setting their positions and rotations. */
		void updateFinger() override;

	protected:
		void setPositions();

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CLeapSkeletalFingerRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CLeapSkeletalFinger();// Serialization only
	};

	/** @} */
}