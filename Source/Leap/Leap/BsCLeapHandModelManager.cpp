//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsCLeapHandModelManager.h"
#include "Leap/BsLeapHandRepresentation.h"
#include "Private/RTTI/BsCLeapHandModelManagerRTTI.h"
#include "Scene/BsSceneManager.h"
#include "Utility/BsUtility.h"

using namespace std::placeholders;

namespace bs
{
	HLeapHandModelBase CLeapHandModelManager::ModelGroup::tryGetModel(LeapModelKind kind, eLeapHandType chirality)
	{
		for (auto it = mModelList.begin(); it != mModelList.end(); ++it)
		{
			HLeapHandModelBase model = *it;
			if (model->getKind() == kind && model->getChirality() == chirality)
			{
				mModelList.erase(it);
				mModelsCheckedOut.push_back(model);
				return model;
			}
		}
		return HLeapHandModelBase();
	}

	void CLeapHandModelManager::ModelGroup::returnToGroup(HLeapHandModelBase model)
	{
		auto iterFind = std::find(mModelsCheckedOut.begin(), mModelsCheckedOut.end(), model);
		mModelsCheckedOut.erase(iterFind);
		mModelList.push_back(model);
		mHandModelManager->mModelToHandRepMapping.erase(model.get());
	}

	CLeapHandModelManager::CLeapHandModelManager()
	{
		setName("LeapHandModelManager");
	}

	CLeapHandModelManager::CLeapHandModelManager(const HSceneObject &parent)
		: Component(parent)
	{
		setName("LeapHandModelManager");
	}

	void CLeapHandModelManager::setLeapProvider(HLeapServiceProvider provider)
	{
		if (mProvider != NULL)
		{
			mOnFixedFrameConn.disconnect();
			mOnUpdateFrameConn.disconnect();
		}

		mProvider = provider;

		if (mProvider != NULL)
		{
			mOnFixedFrameConn = mProvider->onFixedFrame.connect(
				std::bind(&CLeapHandModelManager::onFixedFrame, this, _1));
			mOnUpdateFrameConn = mProvider->onUpdateFrame.connect(
				std::bind(&CLeapHandModelManager::onUpdateFrame, this, _1));
		}
	}

	void CLeapHandModelManager::returnToPool(HLeapHandModelBase model)
	{
		ModelGroup *modelGroup = mModelGroupMapping.at(model.get());
		// First see if there is another active Representation that can use this
		for (int i = 0; i < mActiveHandReps.size(); i++)
		{
			LeapHandRepresentation* rep = mActiveHandReps[i];
			if (rep->getChirality() == model->getChirality() && rep->getKind() == model->getKind())
			{
				bool modelFromGroupFound = false;
				// Represention does not contain a model from modelGroup
				for (int j = 0; j < modelGroup->mModelsCheckedOut.size(); j++)
				{
					HLeapHandModelBase modelToCompare = modelGroup->mModelsCheckedOut[j];
					for (int k = 0; k < rep->mHandModels.size(); k++)
					{
						if (rep->mHandModels[k] == modelToCompare)
							modelFromGroupFound = true;
					}
				}
				if (!modelFromGroupFound)
				{
					rep->registerModel(model);
					mModelToHandRepMapping[model.get()] = rep;
					return;
				}
			}
		}
		// Otherwise return to pool
		modelGroup->returnToGroup(model);
	}

	/** Updates the graphics HandRepresentations. */
	void CLeapHandModelManager::onUpdateFrame(const LeapFrame* frame)
	{
		if (frame != NULL && mGraphicsEnabled)
			_updateHandRepresentations(mGraphicsHandReps, LeapModelKind::Graphics, frame);
	}

	/** Updates the physics HandRepresentations. */
	void CLeapHandModelManager::onFixedFrame(const LeapFrame* frame)
	{
		if (frame != NULL && mPhysicsEnabled)
			_updateHandRepresentations(mPhysicsHandReps, LeapModelKind::Physics, frame);
	}

	void CLeapHandModelManager::_updateHandRepresentations(Map<UINT32, LeapHandRepresentation* > &handReps,
		const LeapModelKind modelType, const LeapFrame* frame)
	{
		for (UINT32 i = 0; i < frame->mNumberOfHands; i++)
		{
			const LeapHand* curHand = &frame->mHands[i];
			LeapHandRepresentation* rep;
			auto it = handReps.find(curHand->mId);
			if (it == handReps.end())
			{
				rep = _createHandRepresentation(curHand, modelType);
				handReps[curHand->mId] = rep;
			}
			else
			{
				rep = it->second;
			}
			if (rep != NULL)
			{
				rep->mIsMarked = true;
				rep->update(curHand);
			}
		}

		/** Mark-and-sweep to finish unused HandRepresentations */
		LeapHandRepresentation *toBeDeleted = NULL;
		for (auto it : handReps)
		{
			LeapHandRepresentation *rep = it.second;
			if (rep->mIsMarked)
			{
				rep->mIsMarked = false;
			}
			else
			{
				// Initialize toBeDeleted with a value to be deleted
				// Debug.Log("Finishing");
				toBeDeleted = rep;
			}
		}
		// Inform the representation that we will no longer be giving it any hand updates because the corresponding 
		// hand has gone away
		if (toBeDeleted != NULL)
		{
			auto it = handReps.find(toBeDeleted->getHandId());
			handReps.erase(it);
			toBeDeleted->finish();
		}
	}

	void CLeapHandModelManager::addNewGroup(String name, HLeapHandModelBase leftModel, HLeapHandModelBase rightModel)
	{
		ModelGroup* group = new ModelGroup;
		group->mGroupName = name;
		group->mLeftModel = leftModel;
		group->mRightModel = rightModel;
		mGroupPool.push_back(group);

		_initializeGroup(group);
	}

	void CLeapHandModelManager::removeGroup(String name)
	{
		auto itFind = std::find_if(mGroupPool.begin(), mGroupPool.end(), [&](auto x) {
			return x->mGroupName == name;
		});

		if (itFind != mGroupPool.end())
			mGroupPool.erase(itFind);
	}

	void CLeapHandModelManager::enableGroup(String name)
	{
		auto itFind = std::find_if(mGroupPool.begin(), mGroupPool.end(), [&](auto x) {
			return x->mGroupName == name;
		});

		if (itFind != mGroupPool.end())
		{
			ModelGroup* group = *itFind;
			for (int hp = 0; hp < mActiveHandReps.size(); hp++)
			{
				LeapHandRepresentation* handRep = mActiveHandReps[hp];
				HLeapHandModelBase model = group->tryGetModel(handRep->getKind(), handRep->getChirality());
				if (model != NULL)
				{
					handRep->registerModel(model);
					mModelToHandRepMapping[model.get()] = handRep;
				}
			}
			group->mIsEnabled = true;
		}
		else
		{
			LOGWRN("A group matching that name does not exisit in the groupPool");
		}
	}

	void CLeapHandModelManager::disableGroup(String name)
	{
		auto itFind = std::find_if(mGroupPool.begin(), mGroupPool.end(), [&](auto x) {
			return x->mGroupName == name;
		});

		if (itFind != mGroupPool.end())
		{
			ModelGroup* group = *itFind;
			for (int m = 0; m < group->mModelsCheckedOut.size(); m++)
			{
				HLeapHandModelBase model = group->mModelsCheckedOut[m];
				auto it = mModelToHandRepMapping.find(model.get());
				if (it != mModelToHandRepMapping.end())
				{
					LeapHandRepresentation* handRep = it->second;
					handRep->removeModel(model);
					group->returnToGroup(model);
					m--;
				}
			}
			group->mIsEnabled = false;
		}
		else
		{
			LOGWRN("A group matching that name does not exisit in the groupPool");
		}
	}

	void CLeapHandModelManager::toggleGroup(String name)
	{
		auto itFind = std::find_if(mGroupPool.begin(), mGroupPool.end(), [&](auto x) {
			return x->mGroupName == name;
		});

		if (itFind != mGroupPool.end())
		{
			ModelGroup* group = *itFind;
			if (group->mIsEnabled == true)
			{
				disableGroup(name);
				group->mIsEnabled = false;
			}
			else
			{
				enableGroup(name);
				group->mIsEnabled = true;
			}
		}
		else
		{
			LOGWRN("A group matching that name does not exisit in the groupPool");
		}
	}

	LeapHandRepresentation* CLeapHandModelManager::_createHandRepresentation(const LeapHand* hand, const LeapModelKind kind)
	{
		LeapHandRepresentation* handRep = new LeapHandRepresentation(this, hand, kind);
		for (ModelGroup* group : mGroupPool)
		{
			if (group->mIsEnabled)
			{
				HLeapHandModelBase model = group->tryGetModel(kind, hand->mType);
				if (model != NULL)
				{
					handRep->registerModel(model);
					auto it = mModelToHandRepMapping.find(model.get());
					if (it == mModelToHandRepMapping.end())
					{
						mModelToHandRepMapping[model.get()] = handRep;
					}
				}
			}
		}
		mActiveHandReps.push_back(handRep);
		return handRep;
	}

	void CLeapHandModelManager::removeHandRepresentation(LeapHandRepresentation *handRepresentation)
	{
		auto it = std::find(mActiveHandReps.begin(), mActiveHandReps.end(), handRepresentation);
		mActiveHandReps.erase(it);
	}

	void CLeapHandModelManager::_initializeProvider()
	{
		if (mProvider != NULL)
			return;

		UINT32 typeId = CLeapServiceProvider::getRTTIStatic()->getRTTIId();
		Vector<HComponent> providers = Utility::findComponents(gSceneManager().getRootNode(), typeId);

		if (providers.size() == 0)
		{
			LOGWRN("Could not find LeapServiceProvider.");
			return;
		}

		mProvider = static_object_cast<CLeapServiceProvider>(providers[0]);
	}

	void CLeapHandModelManager::_initializeGroup(ModelGroup* group)
	{
		// Prevent the ModelGroup be initialized by multiple times
		auto itFind = std::find_if(mModelGroupMapping.begin(), mModelGroupMapping.end(),
			[&](auto x) { return x.second == group; });

		if (itFind != mModelGroupMapping.end())
			return;

		group->mHandModelManager = this;

		HLeapHandModelBase leftModel;
		HLeapHandModelBase rightModel;
		if (group->mIsLeftToBeSpawned)
		{
			HLeapHandModelBase modelToSpawn = group->mLeftModel;
			HSceneObject spawnedGO = modelToSpawn->SO()->clone();
			leftModel = spawnedGO->getComponent<CLeapHandModelBase>();
		}
		else
		{
			leftModel = group->mLeftModel;
		}
		if (leftModel != NULL)
		{
			group->mModelList.push_back(leftModel);
			mModelGroupMapping[leftModel.get()] = group;
		}

		if (group->mIsRightToBeSpawned)
		{
			HLeapHandModelBase modelToSpawn = group->mRightModel;
			HSceneObject spawnedGO = modelToSpawn->SO()->clone();
			rightModel = spawnedGO->getComponent<CLeapHandModelBase>();
		}
		else
		{
			rightModel = group->mRightModel;
		}
		if (rightModel != NULL)
		{
			group->mModelList.push_back(rightModel);
			mModelGroupMapping[rightModel.get()] = group;
		}
	}

	void CLeapHandModelManager::onDisabled()
	{
		mOnFixedFrameConn.disconnect();
		mOnUpdateFrameConn.disconnect();
	}

	void CLeapHandModelManager::onEnabled()
	{
		_initializeProvider();

		if (mProvider == NULL)
			return;

		for (ModelGroup* group : mGroupPool)
			_initializeGroup(group);

		mOnFixedFrameConn = mProvider->onFixedFrame.connect(std::bind(&CLeapHandModelManager::onFixedFrame, this, _1));
		mOnUpdateFrameConn = mProvider->onUpdateFrame.connect(std::bind(&CLeapHandModelManager::onUpdateFrame, this, _1));
	}

	RTTITypeBase* CLeapHandModelManager::getRTTIStatic()
	{
		return CLeapHandModelManagerRTTI::instance();
	}

	RTTITypeBase* CLeapHandModelManager::getRTTI() const
	{
		return CLeapHandModelManager::getRTTIStatic();
	}
}