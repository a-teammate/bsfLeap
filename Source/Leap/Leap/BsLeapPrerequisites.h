//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"

#include "LeapC.h"

/** @addtogroup Layers
*  @{
*/

/** @defgroup Leap Leap
*	Leap Motion input device integration.
*/

/** @} */

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32 // Windows
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_LEAP_EXPORT
#    else
#      if defined(BS_LEAP_EXPORTS)
#        define BS_LEAP_EXPORT __declspec(dllexport)
#      else
#        define BS_LEAP_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_LEAP_EXPORT
#    else
#      if defined(BS_LEAP_EXPORTS)
#        define BS_LEAP_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_LEAP_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_LEAP_HIDDEN
#else // Linux/Mac settings
#  define BS_LEAP_EXPORT __attribute__ ((visibility ("default")))
#  define BS_LEAP_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

namespace bs
{
	class CLeapCapsuleHand;
	class CLeapHandEnableDisable;
	class CLeapHandModel;
	class CLeapHandModelBase;
	class CLeapHandModelManager;
	class CLeapServiceProvider;
}


namespace bs
{
	/** @addtogroup Leap
	*  @{
	*/

	static void* bs_leap_alloc(uint32_t size, eLeapAllocatorType typeHint, void* state)
	{
		state; typeHint;
		return bs_alloc_aligned16(size);
	}

	static void bs_leap_free(void* ptr, void* state)
	{
		state;
		bs_free_aligned16(ptr);
	}

	/** @} */
}

/************************************************************************/
/* 									RTTI                      			*/
/************************************************************************/
namespace bs
{
	enum TypeID_Leap {
		TID_CLeapHandEnableDisable = 81001,
		TID_CLeapHandModel = 81002,
		TID_CLeapHandModelBase = 81003,
		TID_CLeapHandModelManager = 81004,
		TID_CLeapServiceProvider = 81005,
		TID_CLeapCapsuleHand = 81006
	};
}

namespace bs
{
	/** @addtogroup Leap
	*  @{
	*/

	// Game object handles
	typedef GameObjectHandle<CLeapCapsuleHand> HLeapCapsuleHand;
	typedef GameObjectHandle<CLeapHandEnableDisable> HLeapHandEnableDisable;
	typedef GameObjectHandle<CLeapHandModel> HLeapHandModel;
	typedef GameObjectHandle<CLeapHandModelBase> HLeapHandModelBase;
	typedef GameObjectHandle<CLeapHandModelManager> HLeapHandModelManager;
	typedef GameObjectHandle<CLeapServiceProvider> HLeapServiceProvider;

	/** @} */
}