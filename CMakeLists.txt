cmake_minimum_required (VERSION 3.9.0)
project (bsfLeap)

set(BSF_DIRECTORY "${PROJECT_SOURCE_DIR}/../bsf" CACHE STRING "bsf directory")

set (BSF_SOURCE_DIR ${BSF_DIRECTORY}/Source)
set (APP_ROOT_DIR ${BSF_DIRECTORY})
set (SECONDARY_APP_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set (BS_IS_BANSHEE3D 0)

include(${BSF_SOURCE_DIR}/CMake/Properties.cmake)
include(${BSF_SOURCE_DIR}/CMake/FindPackageOrBuild.cmake)
include(${BSF_SOURCE_DIR}/CMake/HelperMethods.cmake)

set (BSF_LEAP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/Source)

include(${BSF_LEAP_SOURCE_DIR}/CMake/Properties.cmake)

add_subdirectory(${BSF_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/bsf)

set(BS_EXAMPLES_BUILTIN_ASSETS_VERSION 7)

# Options
set(BUILD_LEAP_EXAMPLES OFF CACHE BOOL "If true, build targets for running examples will be included in the output.")

# Generate config file
configure_file("${PROJECT_SOURCE_DIR}/Source/CMake/BsExampleConfig.h.in" "${PROJECT_SOURCE_DIR}/Source/Common/BsExampleConfig.h")

if(BUILD_LEAP_EXAMPLES)
	# Check data dependencies
	check_and_update_builtin_assets(bsfExamples ${PROJECT_SOURCE_DIR}/Data ${BS_EXAMPLES_BUILTIN_ASSETS_VERSION} NO)
endif()

# Sub-directories
add_subdirectory(Source/Leap)

if(BUILD_LEAP_EXAMPLES)
	add_subdirectory(Source/Common)
	add_subdirectory(Source/Physics)
endif()
