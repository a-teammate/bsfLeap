//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapFrame.h"
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

	/** The base component class for all hand models. */
	class CLeapHandModelBase : public Component
	{
	public:
		CLeapHandModelBase(const HSceneObject &parent);

		bool getIsTracked() const { return mIsTracked; }

		/** Identifies the kind of representation or this hand. */
		virtual LeapModelKind getKind() const = 0;

		/** Identifies the chirality of this hand. */
		virtual eLeapHandType getChirality() const = 0;

		/**
		 * Implement this function to initialise this hand after it is created.
		 * This function is called by the HandModelManager during the update() phase when a new hand is detected by the
		 * Leap Motion device.
		 */
		virtual void onInitModel() {}

		virtual void begin();

		/**
		 * Implement this function to update this hand once every game loop.
		 * For CLeapHandModel instances assigned to the CLeapHandModelManager graphics hand list, the CLeapHandModelManager
		 * calls this function during the update() phase. For CLeapHandModel instances in the physics hand list, the
		 * CLeapHandModelManager calls this function in the fixedUpdate() phase.
		 */
		virtual void updateFrame() = 0;

		virtual void finishHand();

		/** Returns the Leap hand object represented by this hand model. */
		virtual const LeapHand* getLeapHand() const = 0;

		/**
		 * Assigns a Leap hand object to this hand model.
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