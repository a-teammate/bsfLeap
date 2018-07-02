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

		/** @copydoc CLeapFingerModel::onInitModel */
		void onInitModel() override;

		/** @copydoc CLeapFingerModel::updateFrame */
		void updateFrame() override;

	protected:
		void setPositions();

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