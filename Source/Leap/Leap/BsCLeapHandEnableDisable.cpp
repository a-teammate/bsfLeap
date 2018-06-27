//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsCLeapHandEnableDisable.h"
#include "Leap/BsCLeapHandModel.h"
#include "Private/RTTI/BsCLeapHandEnableDisableRTTI.h"
#include "Scene/BsSceneObject.h"

namespace bs
{
	CLeapHandEnableDisable::CLeapHandEnableDisable()
	{
		setFlag(ComponentFlag::AlwaysRun, true);
		setName("LeapHandEnableDisable");
	}

	CLeapHandEnableDisable::CLeapHandEnableDisable(const HSceneObject &parent)
		: Component(parent)
	{
		setFlag(ComponentFlag::AlwaysRun, true);
		setName("LeapHandEnableDisable");
	}

	void CLeapHandEnableDisable::_handReset()
	{
		SO()->setActive(true);
	}

	void CLeapHandEnableDisable::_handFinish()
	{
		SO()->setActive(false);
	}

	void CLeapHandEnableDisable::onInitialized()
	{
		if (!SO()->hasComponent<CLeapHandModelBase>()) {
			LOGWRN("CLeapHandEnableDisable component requires a CLeapHandModelBase "
				"component attached to the same SceneObject.");
			return;
		}

		mHandModel = SO()->getComponent<CLeapHandModelBase>();

		mHandModel->onBegin.connect(
			std::bind(&CLeapHandEnableDisable::_handReset, this));
		mHandModel->onFinish.connect(
			std::bind(&CLeapHandEnableDisable::_handFinish, this));
	}

	RTTITypeBase* CLeapHandEnableDisable::getRTTIStatic()
	{
		return CLeapHandEnableDisableRTTI::instance();
	}

	RTTITypeBase* CLeapHandEnableDisable::getRTTI() const
	{
		return CLeapHandEnableDisable::getRTTIStatic();
	}
}