# Source files
set(BS_LEAP_INC_COMPONENTS
	"Leap/BsCLeapCapsuleHand.h"
	"Leap/BsCLeapFingerModel.h"
	"Leap/BsCLeapHandEnableDisable.h"
	"Leap/BsCLeapHandModel.h"
	"Leap/BsCLeapHandModelBase.h"
	"Leap/BsCLeapHandModelManager.h"
	"Leap/BsCLeapRigidFinger.h"
	"Leap/BsCLeapRigidHand.h"
	"Leap/BsCLeapServiceProvider.h"
	"Leap/BsCLeapSkeletalFinger.h"
	"Leap/BsCLeapSkeletalHand.h"
)

set(BS_LEAP_INC_RTTI
	"Private/RTTI/BsCLeapCapsuleHandRTTI.h"
	"Private/RTTI/BsCLeapFingerModelRTTI.h"
	"Private/RTTI/BsCLeapHandEnableDisableRTTI.h"
	"Private/RTTI/BsCLeapHandModelRTTI.h"
	"Private/RTTI/BsCLeapHandModelBaseRTTI.h"
	"Private/RTTI/BsCLeapHandModelManagerRTTI.h"
	"Private/RTTI/BsCLeapRigidFingerRTTI.h"
	"Private/RTTI/BsCLeapRigidHandRTTI.h"
	"Private/RTTI/BsCLeapServiceProviderRTTI.h"
	"Private/RTTI/BsCLeapSkeletalFingerRTTI.h"
	"Private/RTTI/BsCLeapSkeletalHandRTTI.h"
)

set(BS_LEAP_INC_UTILITY
	"Utility/BsSmoothedFloat.h"
	"Utility/BsCircularBuffer.h"
)

set(BS_LEAP_INC_NOFILTER
	"Leap/BsLeapDevice.h"
	"Leap/BsLeapFrame.h"
	"Leap/BsLeapFrameAlloc.h"
	"Leap/BsLeapFrameUtility.h"
	"Leap/BsLeapHandRepresentation.h"
	"Leap/BsLeapPrerequisites.h"
	"Leap/BsLeapService.h"
)

set(BS_LEAP_SRC_COMPONENTS
	"Leap/BsCLeapCapsuleHand.cpp"
	"Leap/BsCLeapFingerModel.cpp"
	"Leap/BsCLeapHandEnableDisable.cpp"
	"Leap/BsCLeapHandModel.cpp"
	"Leap/BsCLeapHandModelBase.cpp"
	"Leap/BsCLeapHandModelManager.cpp"
	"Leap/BsCLeapRigidFinger.cpp"
	"Leap/BsCLeapRigidHand.cpp"
	"Leap/BsCLeapServiceProvider.cpp"
	"Leap/BsCLeapSkeletalFinger.cpp"
	"Leap/BsCLeapSkeletalHand.cpp"
)

set(BS_LEAP_SRC_NOFILTER
	"Leap/BsLeapFrameAlloc.cpp"
	"Leap/BsLeapFrameUtility.cpp"
	"Leap/BsLeapHandRepresentation.cpp"
	"Leap/BsLeapService.cpp"
)

source_group("Components" FILES ${BS_LEAP_INC_COMPONENTS} ${BS_LEAP_SRC_COMPONENTS})
source_group("RTTI" FILES ${BS_LEAP_INC_RTTI})
source_group("Utility" FILES ${BS_LEAP_INC_UTILITY})
source_group("" FILES ${BS_LEAP_INC_NOFILTER} ${BS_LEAP_SRC_NOFILTER})

set(BS_LEAP_SRC
	${BS_LEAP_INC_COMPONENTS}
	${BS_LEAP_INC_RTTI}
	${BS_LEAP_INC_UTILITY}
	${BS_LEAP_INC_NOFILTER}
	${BS_LEAP_SRC_COMPONENTS}
	${BS_LEAP_SRC_NOFILTER}
)

# Packages
find_package(Leap REQUIRED)

# Target
add_library(bsfLeap STATIC ${BS_LEAP_SRC})

# Includes
target_include_directories(bsfLeap PUBLIC "./")

# Libraries
## Local libs
target_link_libraries(bsfLeap bsf)

## External lib: Leap
target_link_libraries(bsfLeap ${Leap_LIBRARIES})

# Copy bsf binaries
copyBsfBinaries(bsfLeap ${BSF_DIRECTORY})

# Precompiled header & Unity build
conditional_cotire(bsfLeap)
