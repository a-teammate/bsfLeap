//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Leap/BsCLeapHandModelBase.h"
#include "Private/RTTI/BsCLeapHandModelBaseRTTI.h"

namespace bs
{
	CLeapHandModelBase::CLeapHandModelBase()
	{
	}

	CLeapHandModelBase::CLeapHandModelBase(const HSceneObject &parent)
		: Component(parent)
	{
	}

	void CLeapHandModelBase::begin()
	{
		if (!onBegin.empty()) {
			onBegin();
		}
		mIsTracked = true;
	}

	void CLeapHandModelBase::finish() {
		if (!onFinish.empty())
		{
			onFinish();
		}
		mIsTracked = false;
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* CLeapHandModelBase::getRTTIStatic()
	{
		return CLeapHandModelBaseRTTI::instance();
	}

	RTTITypeBase* CLeapHandModelBase::getRTTI() const
	{
		return CLeapHandModelBase::getRTTIStatic();
	}
}