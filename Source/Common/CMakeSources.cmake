set(BSF_LEAP_COMMON_INC_NOFILTER
	"BsExampleFramework.h"
	"BsCameraFlyer.h"
	"BsObjectRotator.h"
	"BsFPSWalker.h"
	"BsFPSCamera.h"
)

set(BSF_LEAP_COMMON_SRC_NOFILTER
	"BsCameraFlyer.cpp"
	"BsObjectRotator.cpp"
	"BsFPSWalker.cpp"
	"BsFPSCamera.cpp"
)

set(BSF_LEAP_COMMON_SRC
	${BSF_LEAP_COMMON_INC_NOFILTER}
	${BSF_LEAP_COMMON_SRC_NOFILTER}
)
