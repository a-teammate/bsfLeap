//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is 
#pragma once

#include "Leap/BsLeapPrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Leap/BsCLeapHandModel.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class CLeapHandModelRTTI : public RTTIType<CLeapHandModel, CLeapHandModelBase, CLeapHandModelRTTI>
	{
	public:
		CLeapHandModelRTTI()
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
			static String name = "CLeapHandModel";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_CLeapHandModel;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			BS_EXCEPT(InternalErrorException, "Cannot instantiate an abstract class.");
			return nullptr;
		}
	};

	/** @} */
	/** @endcond */
}