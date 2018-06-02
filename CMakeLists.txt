# Project root

cmake_minimum_required(VERSION 3.0)

# Build external dependencies
add_subdirectory(extern/)

# Magma
project(Magma)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Build Magma
add_subdirectory(src/)

