project(rasterizer)

set(MATH_3D_DIR "${CMAKE_SOURCE_DIR}/own_math_3d")
set(SDL_DIR "${DEPENDENCIES_DIRECTORY}/SDL2")

file(GLOB SRC_FILES 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${MATH_3D_DIR}/include" 
    PUBLIC "${SDL_DIR}/include"
)

target_link_directories(${PROJECT_NAME} 
    PUBLIC "${SDL_DIR}/lib/${CMAKE_GENERATOR_PLATFORM}"
    PUBLIC "${MATH_3D_DIR}/lib/${CMAKE_GENERATOR_PLATFORM}"
)

target_link_libraries(${PROJECT_NAME} 
    "SDL2.lib" 
    "SDL2main.lib" 
    "own_math_3d.lib"
)