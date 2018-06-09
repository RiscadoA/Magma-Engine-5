# Magma Debug source

# Get all files
file(GLOB_RECURSE Magma_Resources_Source
    "*.hpp"
    "*.cpp"
)

# Add files as library
add_library(Magma-Resources ${Magma_Resources_Source})
set_target_properties (Magma-Resources PROPERTIES FOLDER Magma)

include_directories(../../)
