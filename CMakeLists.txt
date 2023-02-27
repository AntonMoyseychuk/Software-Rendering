cmake_minimum_required(VERSION 3.3)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()

message("\n[user info] CMAKE_GENERATOR_PLATFORM = ${CMAKE_GENERATOR_PLATFORM}")
message("[user info] CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message("[user info] CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}\n")

set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")
set(SDL_DIR "${DEPENDENCIES_DIR}/vendor/SDL2")
set(IMGUI_DIR "${DEPENDENCIES_DIR}/vendor/imgui")
set(MATH_3D_DIR "${DEPENDENCIES_DIR}/own_math_3d")
set(WINDOW_FRAMEWORK_DIR "${DEPENDENCIES_DIR}/window_framework")

add_subdirectory(raytracer)
#add_subdirectory(rasterizer)
#add_subdirectory(own_math_3d)
#add_subdirectory(window_framework)