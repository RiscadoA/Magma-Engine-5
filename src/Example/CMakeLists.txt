# Example source

# Get all files
file(GLOB_RECURSE Example_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
add_executable(Example ${Example_Source})
set_target_properties (Example PROPERTIES FOLDER Magma)

# Link magma engine
target_link_libraries(Example Magma-Engine)
include_directories(../)
