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
include_directories(../../../extern/glm/)
target_link_libraries(Magma-Graphics glew_s)