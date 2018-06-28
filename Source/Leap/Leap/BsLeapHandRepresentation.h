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
		LeapHandRepresentation(CLeapHandModelManager* parent, const LeapHand* leapHand, const LeapModelKind kind);

		int getHandId() const { return mHandID; }

		eLeapHandType getChirality() const { return mChirality; }

		LeapModelKind getKind() const { return mKind; }

		const LeapHand* getLeapHand() const { return mLeapHand; }

		/** To be called if the HandRepresentation no longer has a LeapHand. */
		void finish();

		void registerModel(HLeapHandModelBase model);

		void removeModel(HLeapHandModelBase model);

		/** Calls updateHand to all registered LeapHandModels. */
		void update(const LeapHand* leapHand);

	public:
		Vector<HLeapHandModelBase> mHandModels;

		bool mIsMarked;

	protected:
		eLeapHandType mChirality;

		LeapModelKind mKind;

		const LeapHand* mLeapHand;

	private:
		CLeapHandModelManager* mParent;

		int mHandID;
	};

	/** @} */
}