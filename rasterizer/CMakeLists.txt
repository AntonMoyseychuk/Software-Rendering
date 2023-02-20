cmake_minimum_required(VERSION 3.3)

set(CMAKE_GENERATOR_PLATFORM win32)

project(rasterizer)

set(MATH_3D_DIR "${CMAKE_SOURCE_DIR}/../own_math_3d")
set(SDL_DIR "${DEPENDENCIES_DIRECTORY}/SDL2")

file(GLOB SRC_FILES 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${MATH_SRC_DIR}/MATH/*.hpp" "${MATH_SRC_DIR}/MATH/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${MATH_3D_DIR}/include" 
    PUBLIC "${SDL_DIR}/include"
)

if (CMAKE_GENERATOR_PLATFORM EQUAL x64)
    target_link_libraries(${PROJECT_NAME} 
        "${SDL_DIR}/lib/x64/SDL2.lib" 
        "${SDL_DIR}/lib/x64/SDL2main.lib"
    )
else()
    target_link_libraries(${PROJECT_NAME} 
        "${SDL_DIR}/lib/x86/SDL2.lib" 
        "${SDL_DIR}/lib/x86/SDL2main.lib" 
        "${MATH_3D_DIR}/build/Debug/own_math_3d.lib"
    )
endif()