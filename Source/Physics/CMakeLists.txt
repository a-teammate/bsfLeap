# Target
if(WIN32)
	add_executable(bsfLeapPhysics WIN32 "Main.cpp")
else()
	add_executable(bsfLeapPhysics "Main.cpp")
endif()

# Working directory
set_target_properties(bsfLeapPhysics PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")

# Libraries
## Local libs
target_link_libraries(bsfLeapPhysics bsfLeapCommon bsfLeap)

# Plugin dependencies
add_engine_dependencies(bsfLeapPhysics)
add_dependencies(bsfLeapPhysics bsfFBXImporter bsfFontImporter bsfFreeImgImporter)

# IDE specific
set_property(TARGET bsfLeapPhysics PROPERTY FOLDER Examples)

# Precompiled header & Unity build
conditional_cotire(bsfLeapPhysics)
