# Source files and their filters
include(CMakeSources.cmake)

# Target
add_library(bsfLeapCommon STATIC ${BSF_LEAP_COMMON_SRC})

# Includes
target_include_directories(bsfLeapCommon PUBLIC "./")

# Libraries
## Local libs
target_link_libraries(bsfLeapCommon bsf)

# IDE specific
set_property(TARGET bsfLeapCommon PROPERTY FOLDER Examples)

# Copy bsf binaries
copyBsfBinaries(bsfLeapCommon ${BSF_DIRECTORY})

# Precompiled header & Unity build
conditional_cotire(bsfLeapCommon)
