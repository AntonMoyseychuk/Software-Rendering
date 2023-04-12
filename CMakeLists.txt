cmake_minimum_required(VERSION 3.3)

# if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
# endif()

set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")
set(SDL_DIR "${DEPENDENCIES_DIR}/SDL")

set(UTILS_DIR "${CMAKE_SOURCE_DIR}/utils")
set(MATH_DIR "${UTILS_DIR}/own-math-3d")
set(WINDOW_DIR "${UTILS_DIR}/window")
set(THREAD_POOL_DIR "${UTILS_DIR}/thread_pool")

add_subdirectory(raytracer)