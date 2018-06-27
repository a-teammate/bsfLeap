//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is 
#pragma once

#include "Reflection/BsRTTIType.h"
#include "Leap/BsCLeapHandEnableDisable.h"
#include "Private/RTTI/BsGameObjectRTTI.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Leap
	 *  @{
	 */

	class CLeapHandEnableDisableRTTI : public RTTIType<CLeapHandEnableDisable, Component, CLeapHandEnableDisableRTTI>
	{
	public:
		CLeapHandEnableDisableRTTI()
		{ }

		void onDeserializationEnded(IReflectable* obj, const UnorderedMap<String, UINT64>& params) override
		{
			// Note: Since this is a CoreObject I should call initialize() right after deserialization,
			// but since this specific type is used in Components we delay initialization until Component
			// itself does it. Keep this is mind in case this ever needs to be deserialized for non-Component
			// purposes (you'll need to call initialize manually).
		}

		const String& getRTTIName() override
		{
			static String name = "CLeapHandEnableDisable";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_CLeapHandEnableDisable;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return GameObjectRTTI::createGameObject<CLeapHandEnableDisable>();
		}
	};

	/** @} */
	/** @endcond */
}