# Magma Core source

# Get all files
file(GLOB_RECURSE Magma_Core_Source
    "*.hpp"
    "*.cpp"
)

# Add files as library
add_library(Magma-Core ${Magma_Core_Source})
set_target_properties (Magma-Core PROPERTIES FOLDER Magma)

include_directories(../../)
