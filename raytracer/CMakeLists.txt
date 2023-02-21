project(raytracer)

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
    "own_math_3d_${CMAKE_GENERATOR_PLATFORM}.lib"
)