//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsCLeapHandModel.h"
#include "Leap/BsCLeapServiceProvider.h"
#include "Leap/BsLeapFrame.h"
#include "Scene/BsComponent.h"

namespace bs
{
	class LeapHandRepresentation;

	/** @addtogroup Leap
	 *  @{
	 */

	/**
	 * The HandModelManager manages a pool of LeapHandModelBases and makes LeapHandRepresentations when it detects a
	 * LeapHand from LeapServiceProvider.
	 *
	 * When a LeapHandRepresentation is created, a LeapHandModelBase is removed from the pool. When a LeapHandRepresentation
	 * is finished, its LeapHandModelBase is returned to the pool.
	 */
	class CLeapHandModelManager : public Component
	{
	public:
		/**
		 * ModelGroup contains a left/right pair of LeapHandModelBases
		 * @param mModelList The LeapHandModelBases available for use by LeapHandRepresentations
		 * @param mModelsCheckedOut The HandModelBases currently in use by active HandRepresentations
		 * @param mIsEnabled determines whether the ModelGroup is active at app Start(), though ModelGroup's are controlled
		 * with the EnableGroup & DisableGroup methods.
		 */
		class ModelGroup
		{
		public:
			String mGroupName;

			HLeapHandModelBase mLeftModel;
			HLeapHandModelBase mRightModel;
			bool mIsLeftToBeSpawned = false;
			bool mIsRightToBeSpawned = false;

			Vector<HLeapHandModelBase> mModelList;
			Vector<HLeapHandModelBase> mModelsCheckedOut;

			bool mIsEnabled = true;
			CLeapHandModelManager* mHandModelManager = NULL;

			/*
			 * Looks for suitable HandModelBase is the ModelGroup's modelList, if found, it is added to modelsCheckedOut. 
			 * If not, one can be cloned.
			 */
			HLeapHandModelBase tryGetModel(LeapModelKind kind, eLeapHandType chirality);

			void returnToGroup(HLeapHandModelBase model);
		};

	public:
		/** Construct CLeapHandModelManager. */
		CLeapHandModelManager(const HSceneObject &parent);

		/** Returns the LeapServiceProvider used to drive hands. */
		HLeapServiceProvider getLeapProvider() const { return mProvider; }

		/** Sets the LeapServiceProvider used to drive hands. */
		void setLeapProvider(HLeapServiceProvider provider);

		void returnToPool(HLeapHandModelBase model);

		void removeHandRepresentation(LeapHandRepresentation *handRepresentation);

		void addNewGroup(String name, HLeapHandModelBase leftModel, HLeapHandModelBase rightModel);

		void removeGroup(String name);

		// T GetHandModel<T>(int handId) where T : HandModelBase {
		//  foreach (ModelGroup group in mGroupPool) {
		//    foreach (HandModelBase handModel in group.modelsCheckedOut) {
		//      if (handModel.GetLeapHand().Id == handId && handModel is T) {
		//        return handModel as T;
		//      }
		//    }
		//  }
		//  return NULL;
		//}

		/**
		* EnableGroup finds suitable HandRepresentations and adds HandModelBases from the ModelGroup, returns them to
		* their ModelGroup and sets the groups IsEnabled to true.
		* @param name Takes a string that matches the ModelGroup's name
		* serialized in the Inspector
		*/
		void enableGroup(String name);

		/**
		* DisableGroup finds and removes the ModelGroup's HandModelBases from their HandRepresentations, returns them
		* to their ModelGroup and sets the groups IsEnabled to false.
		* @param name Takes a string that matches the ModelGroup's name serialized in the Inspector
		*/
		void disableGroup(String name);

		void toggleGroup(String name);

	protected:
		/** Updates the graphics HandRepresentations. */
		virtual void onUpdateFrame(const LeapFrame* frame);

		/** Updates the physics HandRepresentations. */
		virtual void onFixedFrame(const LeapFrame* frame);

		/**
		* Receives a LeapHand and combines that with a LeapHandModelBase to create a LeapHandRepresentation.
		* @param hand The LeapHand data to drive a LeapHandModelBase
		* @param modelType Filters for a type of hand model, for example, physics or graphics hands.
		*/
		LeapHandRepresentation *_createHandRepresentation(const LeapHand *hand, const LeapModelKind modelType);

		/**
		 * Updates LeapHandRepresentations based in the specified HandRepresentation Dictionary.
		 * Active LeapHandRepresentation instances are updated if the hand they represent is still present in the
		 * LeapServiceProvider's CurrentFrame. Otherwise, the LeapHandRepresentation is removed.
		 * If new LeapHand objects are present in the frame, new HandRepresentations are
		 * created and added to the dictionary.
		 * @param handReps = A dictionary of LeapHand ID's with a paired HandRepresentation
		 * @param modelType Filters for a type of hand model, for example, physics or graphics hands.
		 * @param frame The LeapFrame containing LeapHand data for each currently tracked hand
		 */
		virtual void _updateHandRepresentations(Map<UINT32, LeapHandRepresentation* > &handReps,
			const LeapModelKind modelType, const LeapFrame* frame);

	private:
		void _initializeProvider();

		void _initializeGroup(ModelGroup* group);

	public:
		bool mGraphicsEnabled = true;
		bool mPhysicsEnabled = true;

	protected:
		Map<UINT32, LeapHandRepresentation*> mGraphicsHandReps;
		Map<UINT32, LeapHandRepresentation*> mPhysicsHandReps;

	private:
		Vector<ModelGroup*> mGroupPool;

		Vector<LeapHandRepresentation*> mActiveHandReps;

		Map<CLeapHandModelBase*, ModelGroup*> mModelGroupMapping;
		Map<CLeapHandModelBase*, LeapHandRepresentation*> mModelToHandRepMapping;

		HLeapServiceProvider mProvider;

		HEvent mOnFixedFrameConn;
		HEvent mOnUpdateFrameConn;

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

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