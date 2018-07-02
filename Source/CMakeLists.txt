# Version
set (BSF_LEAP_VERSION_MAJOR 1)
set (BSF_LEAP_VERSION_MINOR 0)

include(CMake/Properties.cmake)

# Options
set(BUILD_BSF_LEAP_EXAMPLES OFF CACHE BOOL "If true, build targets for running examples will be included in the output.")

if(BUILD_BSF_LEAP_EXAMPLES)
	set(BS_EXAMPLES_BUILTIN_ASSETS_VERSION 7)

	# Generate config file
	configure_file("CMake/BsExampleConfig.h.in" "Common/BsExampleConfig.h")

	# Check data dependencies
	check_and_update_builtin_assets(bsfExamples ${PROJECT_SOURCE_DIR}/Data ${BS_EXAMPLES_BUILTIN_ASSETS_VERSION} NO)
endif()

# Sub-directories
add_subdirectory(Leap)

if(BUILD_BSF_LEAP_EXAMPLES)
	add_subdirectory(Common)
	add_subdirectory(Physics)
endif()
