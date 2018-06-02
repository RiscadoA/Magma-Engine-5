# Magma Engine source

# Get all files
file(GLOB_RECURSE Magma_Engine_Source
    "*.hpp"
    "*.cpp"
)

# Add files as library
add_library(Magma-Engine ${Magma_Engine_Source})
set_target_properties (Magma-Engine PROPERTIES FOLDER Magma)

include_directories(../../)
include_directories(../../../extern/glm/)

target_link_libraries(Magma-Engine Magma-Debug)
target_link_libraries(Magma-Engine Magma-Core)
target_link_libraries(Magma-Engine Magma-Graphics)
target_link_libraries(Magma-Engine Magma-Input)
