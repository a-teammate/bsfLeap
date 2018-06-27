# Find Leap
#
# This module defines
#  LeapC_INCLUDE_DIRS
#  LeapC_LIBRARIES
#  LeapC_FOUND

start_find_package(Leap)

set(Leap_INSTALL_DIR ${PROJECT_SOURCE_DIR}/Dependencies/Leap CACHE PATH "")
gen_default_lib_search_dirs(Leap)

find_imported_includes(Leap LeapC.h)
find_imported_library_shared(Leap LeapC)

install_dependency_binaries(Leap)

end_find_package(Leap LeapC)
