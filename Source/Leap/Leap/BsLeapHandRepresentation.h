//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsCLeapHandModel.h"

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	 */

	class CLeapHandModelManager;

	/**
	* LeapHandRepresentation is a container class that facillitates the HandModelBase lifecycle.
	*/
	class LeapHandRepresentation
	{
	public:
		LeapHandRepresentation(CLeapHandModelManager *parent, LeapHand *hand, eLeapHandType chirality, LeapModelType type);

		int getHandId() const { return mHandID; }

		eLeapHandType getChirality() const { return mChirality; }

		LeapModelType getType() const { return mType; }

		LeapHand* getMostRecentHand() const { return mMostRecentHand; }

		/** To be called if the HandRepresentation no longer has a LeapHand. */
		void finish();

		void addModel(HLeapHandModelBase model);

		void removeModel(HLeapHandModelBase model);

		/** Calls Updates in HandModelBases. */
		void update(LeapHand* hand);

	public:
		Vector<HLeapHandModelBase> mHandModels;

		int mLastUpdatedTime;

		bool mIsMarked;

	protected:
		eLeapHandType mChirality;

		LeapModelType mType;

		LeapHand* mMostRecentHand;

	private:
		CLeapHandModelManager* mParent;

		int mHandID;
	};

	/** @} */
}