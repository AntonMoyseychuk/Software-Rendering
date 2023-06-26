cmake_minimum_required(VERSION 3.8)

set(PROJECT_DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")
set(PROJECT_UTIL_DIR "${CMAKE_SOURCE_DIR}/utils")

add_subdirectory(rasterizer)
# add_subdirectory(raytracer)