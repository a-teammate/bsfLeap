//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsCLeapHandModelManager.h"
#include "Leap/BsLeapHandRepresentation.h"
#include "Private/RTTI/BsCLeapHandModelManagerRTTI.h"
#include "Scene/BsSceneManager.h"

using namespace std::placeholders;

namespace bs
{
	HLeapHandModelBase CLeapHandModelManager::ModelGroup::tryGetModel(eLeapHandType chirality, LeapModelType modelType)
	{
		for (auto it = mModelList.begin(); it != mModelList.end(); ++it)
		{
			HLeapHandModelBase model = *it;
			if (model->getChirality() == chirality && model->getType() == modelType)
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
		setFlag(ComponentFlag::AlwaysRun, true);
	}

	CLeapHandModelManager::CLeapHandModelManager(const HSceneObject &parent)
		: Component(parent)
	{
		setName("LeapHandModelManager");
		setFlag(ComponentFlag::AlwaysRun, true);
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
			LeapHandRepresentation *rep = mActiveHandReps[i];
			if (rep->getChirality() == model->getChirality() && rep->getType() == model->getType())
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
					rep->addModel(model);
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
			updateHandRepresentations(mGraphicsHandReps, LeapModelType::Graphics, frame);
	}

	/** Updates the physics HandRepresentations. */
	void CLeapHandModelManager::onFixedFrame(const LeapFrame* frame)
	{
		if (frame != NULL && mPhysicsEnabled)
			updateHandRepresentations(mPhysicsHandReps, LeapModelType::Physics, frame);
	}

	void CLeapHandModelManager::updateHandRepresentations(Map<int, LeapHandRepresentation *> &handReps,
		LeapModelType modelType, const LeapFrame* frame)
	{
		for (UINT32 i = 0; i < frame->mNumberOfHands; i++)
		{
			LeapHand *curHand = &frame->mHands[i];
			LeapHandRepresentation *rep;
			auto it = handReps.find(curHand->mId);
			if (it == handReps.end())
			{
				rep = makeHandRepresentation(curHand, modelType);
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
				rep->mLastUpdatedTime = (int)frame->mFramerate;
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

	void CLeapHandModelManager::initializeModelGroup(ModelGroup *group)
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

	void CLeapHandModelManager::addNewGroup(String groupName, HLeapHandModelBase leftModel, HLeapHandModelBase rightModel)
	{
		ModelGroup *group = new ModelGroup;
		group->mGroupName = groupName;
		group->mLeftModel = leftModel;
		group->mRightModel = rightModel;
		mModelPool.push_back(group);

		initializeModelGroup(group);
	}

	void CLeapHandModelManager::removeGroup(String groupName)
	{
		auto it = std::find_if(mModelPool.begin(), mModelPool.end(),
			[&](auto x) { return x->mGroupName == groupName; });
		if (it != mModelPool.end()) {
			mModelPool.erase(it);
		}
	}

	void CLeapHandModelManager::enableGroup(String groupName)
	{
		ModelGroup* group = NULL;
		for (int i = 0; i < mModelPool.size(); i++)
		{
			if (mModelPool[i]->mGroupName == groupName)
			{
				group = mModelPool[i];
				for (int hp = 0; hp < mActiveHandReps.size(); hp++)
				{
					LeapHandRepresentation* handRep = mActiveHandReps[hp];
					HLeapHandModelBase model = group->tryGetModel(handRep->getChirality(), handRep->getType());
					if (model != NULL)
					{
						handRep->addModel(model);
						mModelToHandRepMapping[model.get()] = handRep;
					}
				}
				group->mIsEnabled = true;
			}
		}
		if (group == NULL) {
			LOGWRN("A group matching that name does not exisit in the modelPool");
		}
	}

	void CLeapHandModelManager::disableGroup(String groupName)
	{
		ModelGroup* group = NULL;
		for (int i = 0; i < mModelPool.size(); i++)
		{
			if (mModelPool[i]->mGroupName == groupName)
			{
				group = mModelPool[i];
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
				// Assert.AreEqual(0, group->mModelsCheckedOut.size(),
				//                group->mGroupName + "'s modelsCheckedOut List has not been cleared");
				group->mIsEnabled = false;
			}
		}
		if (group == NULL) {
			LOGWRN("A group matching that name does not exisit in the modelPool");
		}
	}

	void CLeapHandModelManager::toggleGroup(String groupName)
	{
		auto it = std::find_if(mModelPool.begin(), mModelPool.end(), [&](auto x) { return x->mGroupName == groupName; });
		if (it != mModelPool.end())
		{
			ModelGroup *modelGroup = *it;
			if (modelGroup->mIsEnabled == true)
			{
				disableGroup(groupName);
				modelGroup->mIsEnabled = false;
			}
			else
			{
				enableGroup(groupName);
				modelGroup->mIsEnabled = true;
			}
		}
		else
		{
			LOGWRN("A group matching that name does not exisit in the modelPool");
		}
	}

	LeapHandRepresentation* CLeapHandModelManager::makeHandRepresentation(LeapHand* hand, LeapModelType modelType)
	{
		LeapHandRepresentation *handRep = new LeapHandRepresentation(this, hand, hand->mType, modelType);
		for (ModelGroup *group : mModelPool)
		{
			if (group->mIsEnabled)
			{
				HLeapHandModelBase model = group->tryGetModel(hand->mType, modelType);
				if (model != NULL)
				{
					handRep->addModel(model);
					auto it = mModelToHandRepMapping.find(model.get());
					if (it == mModelToHandRepMapping.end())
					{
						// model->group = group;
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

	void CLeapHandModelManager::onInitialized()
	{
		if (mProvider != NULL)
			return;

		Vector<HLeapServiceProvider> arrayProvider = gSceneManager().findComponents<CLeapServiceProvider>();

		if (arrayProvider.size() == 0)
			LOGERR("Couldn't find CLeapServiceProvider component in the scene.");

		mProvider = arrayProvider[0];
	}

	void CLeapHandModelManager::onDisabled()
	{
		mOnFixedFrameConn.disconnect();
		mOnUpdateFrameConn.disconnect();
	}

	void CLeapHandModelManager::onEnabled()
	{
		mOnFixedFrameConn = mProvider->onFixedFrame.connect(
			std::bind(&CLeapHandModelManager::onFixedFrame, this, _1));
		mOnUpdateFrameConn = mProvider->onUpdateFrame.connect(
			std::bind(&CLeapHandModelManager::onUpdateFrame, this, _1));
	}

	RTTITypeBase* CLeapHandModelManager::getRTTIStatic()
	{
		return CLeapHandModelManagerRTTI::instance();
	}

	RTTITypeBase* CLeapHandModelManager::getRTTI() const
	{
		return CLeapHandModelManager::getRTTIStatic();
	}

	//	void CLeapHandModelManager::setMappingX(float minX, float maxX, float outputMinX, float outputMaxX)
	//	{
	//		mOffsetInX = minX;
	//		mOffsetOutX = outputMinX;
	//		mScaleX = (outputMaxX - outputMinX) / (maxX - minX);
	//	}
	//
	//	void CLeapHandModelManager::setMappingY(float minY, float maxY, float outputMinY, float outputMaxY)
	//	{
	//		mOffsetInY = minY;
	//		mOffsetOutY = outputMinY;
	//		mScaleY = (outputMaxY - outputMinY) / (maxY - minY);
	//	}
	//
	//	void CLeapHandModelManager::setMappingZ(float minZ, float maxZ, float outputMinZ, float outputMaxZ)
	//	{
	//		mOffsetInZ = minZ;
	//		mOffsetOutZ = outputMinZ;
	//		mScaleZ = (outputMaxZ - outputMinZ) / (maxZ - minZ);
	//	}
	//
	//	Vector3 CLeapHandModelManager::toVecTransform(LEAP_VECTOR v)
	//	{
	//		Vector3 p = toVec(v);
	//		p.x = mOffsetOutX + (p.x - mOffsetInX) * mScaleX;
	//		p.y = mOffsetOutY + (p.y - mOffsetInY) * mScaleY;
	//		p.z = mOffsetOutZ + (p.z - mOffsetInZ) * mScaleZ;
	//
	//		return p;
	//	}
}