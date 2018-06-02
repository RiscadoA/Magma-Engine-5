# Magma Debug source

# Get all files
file(GLOB_RECURSE Magma_Debug_Source
    "*.hpp"
    "*.cpp"
)

# Add files as library
add_library(Magma-Debug ${Magma_Debug_Source})
set_target_properties (Magma-Debug PROPERTIES FOLDER Magma)

include_directories(../../)
