cmake_minimum_required(VERSION 3.3)


project(software_rendering)

message("\nCMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()
message("CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}\n")

file(GLOB SRC_FILES 
    "${PROJECT_SOURCE_DIR}/src/rasterizer/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/raytracer/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/math/*.hpp" "${PROJECT_SOURCE_DIR}/src/math/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")