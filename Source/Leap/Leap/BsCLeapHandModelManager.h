//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsCLeapHandModel.h"
#include "Leap/BsCLeapServiceProvider.h"
#include "Leap/BsLeapTypes.h"
#include "Scene/BsComponent.h"

namespace bs
{
	class LeapHandRepresentation;

	/** @addtogroup Leap
	 *  @{
	 */

	/**
	 * The HandModelManager manages a pool of LeapHandModelBases and makes
	 * HandRepresentations when it detects a Leap Hand from LeapProvider.
	 *
	 * When a LeapHandRepresentation is created, a HandModelBase is removed from the
	 * pool. When a HandRepresentation is finished, its HandModelBase is returned to
	 * the pool.
	 */
	class CLeapHandModelManager : public Component
	{
	public:
		/**
		 * ModelGroup contains a left/right pair of HandModelBase's
		 * @param modelList The HandModelBases available for use by
		 * HandRepresentations
		 * @param modelsCheckedOut The HandModelBases currently in use by active
		 * HandRepresentations
		 * @param IsEnabled determines whether the ModelGroup is active at app
		 * Start(), though ModelGroup's are controlled with the EnableGroup() &
		 * DisableGroup methods.
		 */
		class ModelGroup
		{
		public:
			String mGroupName;
			CLeapHandModelManager* mHandModelManager;

			HLeapHandModelBase mLeftModel;
			bool mIsLeftToBeSpawned;
			HLeapHandModelBase mRightModel;
			bool mIsRightToBeSpawned;

			Vector<CLeapHandModelBase *> mModelList;
			Vector<CLeapHandModelBase *> mModelsCheckedOut;

			bool mIsEnabled = true;

			/*Looks for suitable HandModelBase is the ModelGroup's modelList, if found,
				* it is added to modelsCheckedOut. If not, one can be cloned */
			CLeapHandModelBase *tryGetModel(eLeapHandType chirality, LeapModelType modelType);

			void returnToGroup(CLeapHandModelBase *model);
		};

	public:
		/** Construct CLeapHandModelManager. */
		CLeapHandModelManager(const HSceneObject &parent);

		/** Returns the LeapServiceProvider used to drive hands. */
		HLeapServiceProvider getLeapProvider() const { return mProvider; }

		/** Sets the LeapServiceProvider used to drive hands. */
		void setLeapProvider(HLeapServiceProvider provider);

		void returnToPool(CLeapHandModelBase *model);

		/**
		 * MakeHandRepresentation receives a Hand and combines that with a
		 * HandModelBase to create a HandRepresentation
		 * @param hand The Leap Hand data to be drive a HandModelBase
		 * @param modelType Filters for a type of hand model, for example, physics or
		 * graphics hands.
		 */
		LeapHandRepresentation *makeHandRepresentation(LeapHand *hand, LeapModelType modelType);

		void removeHandRepresentation(LeapHandRepresentation *handRepresentation);

	protected:
		/** Updates the graphics HandRepresentations. */
		virtual void onUpdateFrame(const LeapFrame* frame);

		/** Updates the physics HandRepresentations. */
		virtual void onFixedFrame(const LeapFrame* frame);

		/**
		* Updates HandRepresentations based in the specified HandRepresentation
		* Dictionary. Active HandRepresentation instances are updated if the hand
		* they represent is still present in the Provider's CurrentFrame; otherwise,
		* the HandRepresentation is removed. If new Leap Hand objects are present in
		* the Leap HandRepresentation Dictionary, new HandRepresentations are created
		* and added to the dictionary.
		* @param handReps = A dictionary of Leap Hand ID's with a paired
		* HandRepresentation
		* @param modelType Filters for a type of hand model, for example, physics or
		* graphics hands.
		* @param frame The Leap Frame containing Leap Hand data for each currently
		* tracked hand
		*/
		virtual void
		updateHandRepresentations(Map<int, LeapHandRepresentation *> &handReps,
			LeapModelType modelType, const LeapFrame* frame);

		/** Popuates the ModelPool with the contents of the ModelCollection */
		void start() {
			for (int i = 0; i < mModelPool.size(); i++) {
				initializeModelGroup(mModelPool[i]);
			}
		}

	private:
		void initializeModelGroup(ModelGroup *group);

	public:
		void addNewGroup(String groupName, HLeapHandModelBase leftModel,
			HLeapHandModelBase rightModel);

		void removeGroup(String groupName);

		// T GetHandModel<T>(int handId) where T : HandModelBase {
		//  foreach (ModelGroup group in mModelPool) {
		//    foreach (HandModelBase handModel in group.modelsCheckedOut) {
		//      if (handModel.GetLeapHand().Id == handId && handModel is T) {
		//        return handModel as T;
		//      }
		//    }
		//  }
		//  return NULL;
		//}

		/**
		* EnableGroup finds suitable HandRepresentations and adds HandModelBases from
		* the ModelGroup, returns them to their ModelGroup and sets the groups
		* IsEnabled to true.
		* @param groupName Takes a string that matches the ModelGroup's groupName
		* serialized in the Inspector
		*/
		void enableGroup(String groupName);

		/**
		* DisableGroup finds and removes the ModelGroup's HandModelBases from their
		* HandRepresentations, returns them to their ModelGroup and sets the groups
		* IsEnabled to false.
		* @param groupName Takes a string that matches the ModelGroup's groupName
		* serialized in the Inspector
		*/
		void disableGroup(String groupName);

		void toggleGroup(String groupName);

	public:
		bool mGraphicsEnabled = true;
		bool mPhysicsEnabled = true;

	protected:
		Map<int, LeapHandRepresentation *> mGraphicsHandReps;
		Map<int, LeapHandRepresentation *> mPhysicsHandReps;

	private:
		Vector<ModelGroup *> mModelPool;

		Vector<LeapHandRepresentation *> mActiveHandReps;

		Map<CLeapHandModelBase *, ModelGroup *> mModelGroupMapping;
		Map<CLeapHandModelBase *, LeapHandRepresentation *> mModelToHandRepMapping;

		HLeapServiceProvider mProvider;

		HEvent mOnFixedFrameConn;
		HEvent mOnUpdateFrameConn;

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		/** @copydoc Component::onInitialized */
		void onInitialized() override;

		/** @copydoc Component::onDisabled */
		void onDisabled() override;

		/** @copydoc Component::onEnabled */
		void onEnabled() override;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CLeapHandEnableDisableRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CLeapHandModelManager();// Serialization only
	};

	/** @} */
}