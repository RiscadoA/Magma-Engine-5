# Magma Core source

# Get all files
file(GLOB_RECURSE Magma_Graphics_Source
    "*.hpp"
    "*.cpp"
)

# Add files as library
add_library(Magma-Graphics ${Magma_Graphics_Source})
set_target_properties (Magma-Graphics PROPERTIES FOLDER Magma)

include_directories(../../)
include_directories(../../../extern/glew-cmake/include/)
include_directories(../../../extern/freetype/include/)
include_directories(../../../extern/glm/)
include_directories(../../../extern/openvr/headers/)
target_link_libraries(Magma-Graphics glew_s)
target_link_libraries(Magma-Graphics freetype)
target_link_libraries(Magma-Graphics openvr_api)
target_link_libraries(Magma-Graphics Magma-Core)
target_link_libraries(Magma-Graphics Magma-Resources)