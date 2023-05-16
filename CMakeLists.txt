cmake_minimum_required(VERSION 3.3)

set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")

set(SDL_DIR "${DEPENDENCIES_DIR}/SDL")
set(STB_DIR "${DEPENDENCIES_DIR}/vendor")

set(UTILS_DIR "${CMAKE_SOURCE_DIR}/utils")
set(MATH_DIR "${UTILS_DIR}/own-math-3d")
set(WINDOW_DIR "${UTILS_DIR}/window")
set(THREAD_POOL_DIR "${UTILS_DIR}/thread_pool")
set(PARSERS_DIR "${UTILS_DIR}/parsers")

add_subdirectory(rasterizer)
# add_subdirectory(raytracer)