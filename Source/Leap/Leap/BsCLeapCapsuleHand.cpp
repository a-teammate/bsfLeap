//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsCLeapCapsuleHand.h"
#include "Private/RTTI/BsCLeapCapsuleHandRTTI.h"

namespace bs
{
	CLeapCapsuleHand::CLeapCapsuleHand()
	{
		setName("LeapCapsuleHand");
	}

	CLeapCapsuleHand::CLeapCapsuleHand(const HSceneObject &parent)
		: CLeapHandModelBase(parent)
	{
		setName("LeapCapsuleHand");
	}

	RTTITypeBase* CLeapCapsuleHand::getRTTIStatic()
	{
		return CLeapCapsuleHandRTTI::instance();
	}

	RTTITypeBase* CLeapCapsuleHand::getRTTI() const
	{
		return CLeapCapsuleHand::getRTTIStatic();
	}
}