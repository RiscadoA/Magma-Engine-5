# Magma Core source

# Get all files
file(GLOB_RECURSE Magma_Input_Source
    "*.hpp"
    "*.cpp"
)

# Add files as library
add_library(Magma-Input ${Magma_Input_Source})
set_target_properties (Magma-Input PROPERTIES FOLDER Magma)

include_directories(../../)

include_directories(../../../extern/glfw/include/)
target_link_libraries(Magma-Input glfw)

