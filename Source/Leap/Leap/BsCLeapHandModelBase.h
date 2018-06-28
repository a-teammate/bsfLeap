//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapTypes.h"
#include "Scene/BsComponent.h"

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	 */

	/**
	 * Leap hand model type.
	 */
	enum class LeapModelKind
	{
		Graphics,
		Physics,
	};

	/**
	 * Abstract base class for building Leap hand models.
	 */
	class CLeapHandModelBase : public Component
	{
	public:
		/** Construct CLeapCapsuleHand. */
		CLeapHandModelBase(const HSceneObject &parent);

		bool getIsTracked() const { return mIsTracked; }

		virtual LeapModelKind getKind() const = 0;

		virtual eLeapHandType getChirality() const = 0;

		/**
		 * Implement this function to initialise this hand after it is created.
		 * This function is called by the HandModelManager during the Update()
		 * phase when a new hand is detected by the Leap Motion device.
		 */
		virtual void initHand() {}

		virtual void begin();

		/**
		 * Implement this function to update this hand once every game loop.
		 * For HandModel instances assigned to the HandController graphics hand list,
		 * the HandController calls this function during the Update() phase. For
		 * HandModel instances in the physics hand list, the HandModelManager calls
		 * this function in the FixedUpdate() phase.
		 */
		virtual void updateHand() = 0;

		virtual void finishHand();

		/**
		 * Returns the Leap Hand object represented by this HandModel.
		 * Note that any physical quantities and directions obtained from the
		 * Leap Hand object are relative to the Leap Motion coordinate system,
		 * which uses a right-handed axes and units of millimeters.
		 */
		virtual const LeapHand* getLeapHand() const = 0;

		/**
		 * Assigns a Leap Hand object to this hand model.
		 * Note that the Leap Hand objects are recreated every frame. The parent
		 * HandController calls this method to set or update the underlying hand.
		 */
		virtual void setLeapHand(const LeapHand* hand) = 0;

		Event<void()> onBegin;
		Event<void()> onFinish;

	private:
		bool mIsTracked = false;

		/************************************************************************/
		/* 								SERIALIZATION                      		*/
		/************************************************************************/
	protected:
		CLeapHandModelBase();// Serialization only

	public:
		friend class CLeapHandModelBaseRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;
	};

	/** @} */
}