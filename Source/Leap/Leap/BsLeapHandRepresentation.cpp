//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsLeapHandRepresentation.h"
#include "Leap/BsCLeapHandModelManager.h"

namespace bs
{
	LeapHandRepresentation::LeapHandRepresentation(CLeapHandModelManager* parent, const LeapHand* leapHand,
		LeapModelKind kind)
	{
		mParent = parent;
		mHandID = leapHand->mId;
		mChirality = leapHand->mType;
		mKind = kind;
		mLeapHand = leapHand;
	}

	void LeapHandRepresentation::finish()
	{
		for (int i = 0; i < mHandModels.size(); i++)
		{
			mHandModels[i]->finishHand();
			mParent->returnToPool(mHandModels[i]);
			mHandModels[i] = NULL;
		}
		mParent->removeHandRepresentation(this);
	}

	void LeapHandRepresentation::registerModel(HLeapHandModelBase model)
	{
		mHandModels.push_back(model);
		if (model->getLeapHand() == NULL)
		{
			model->setLeapHand(mLeapHand);
			model->initHand();
			model->begin();
			model->updateHand();
		}
		else
		{
			model->setLeapHand(mLeapHand);
			model->begin();
		}
	}

	void LeapHandRepresentation::removeModel(HLeapHandModelBase model)
	{
		model->finishHand();
		auto it = std::find(mHandModels.begin(), mHandModels.end(), model);
		mHandModels.erase(it);
	}

	void LeapHandRepresentation::update(const LeapHand* leapHand)
	{
		mLeapHand = leapHand;
		for (int i = 0; i < mHandModels.size(); i++)
		{
			mHandModels[i]->setLeapHand(leapHand);
			mHandModels[i]->updateHand();
		}
	}
}