//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Leap/BsCLeapHandModel.h"
#include "Private/RTTI/BsCLeapHandModelRTTI.h"

namespace bs
{
	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* CLeapHandModel::getRTTIStatic()
	{
		return CLeapHandModelRTTI::instance();
	}

	RTTITypeBase* CLeapHandModel::getRTTI() const
	{
		return CLeapHandModel::getRTTIStatic();
	}
}
