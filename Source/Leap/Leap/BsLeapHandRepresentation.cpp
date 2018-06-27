//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsLeapHandRepresentation.h"
#include "Leap/BsCLeapHandModelManager.h"

namespace bs {

LeapHandRepresentation::LeapHandRepresentation(CLeapHandModelManager *parent,
                                               LeapHand *hand,
                                               eLeapHandType chirality,
                                               LeapModelType type) {
  mParent = parent;
  mHandID = hand->mId;
  mChirality = chirality;
  mType = type;
  mMostRecentHand = hand;
}

void LeapHandRepresentation::finish() {
  for (int i = 0; i < mHandModels.size(); i++) {
    mHandModels[i]->finish();
    mParent->returnToPool(mHandModels[i]);
    mHandModels[i] = NULL;
  }
  mParent->removeHandRepresentation(this);
}

void LeapHandRepresentation::addModel(CLeapHandModelBase *model) {
  mHandModels.push_back(model);
  if (model->getLeapHand() == NULL) {
    model->setLeapHand(mMostRecentHand);
    model->init();
    model->begin();
    model->update();
  } else {
    model->setLeapHand(mMostRecentHand);
    model->begin();
  }
}

void LeapHandRepresentation::removeModel(CLeapHandModelBase *model) {
  model->finish();
  auto it = std::find(mHandModels.begin(), mHandModels.end(), model);
  mHandModels.erase(it);
}

void LeapHandRepresentation::update(LeapHand *hand) {
  mMostRecentHand = hand;
  for (int i = 0; i < mHandModels.size(); i++) {
    mHandModels[i]->setLeapHand(hand);
    mHandModels[i]->update();
  }
}

}
