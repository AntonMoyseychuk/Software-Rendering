cmake_minimum_required(VERSION 3.3)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()

message("[user info] CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message("[user info] CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

set(DEPENDENCIES_DIRECTORY "${CMAKE_SOURCE_DIR}/dependencies")

add_subdirectory(rasterizer)
#add_subdirectory(raytracer)