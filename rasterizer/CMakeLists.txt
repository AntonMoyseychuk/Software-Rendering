project(rasterizer)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC_FILES 
    "${PROJECT_SOURCE_DIR}/app/*.cpp"
    "${PROJECT_SOURCE_DIR}/graphics/*.cpp"

    "${MATH_DIR}/math_3d/*.cpp"
    "${WINDOW_DIR}/window/*.cpp"
    "${THREAD_POOL_DIR}/thread_pool/*.cpp"
    "${PARSERS_DIR}/obj/*.cpp"
    "${PARSERS_DIR}/tga/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${PROJECT_SOURCE_DIR}"
    
    PUBLIC "${SDL_DIR}/include"
    PUBLIC "${STB_DIR}"
    
    PUBLIC "${WINDOW_DIR}"
    PUBLIC "${THREAD_POOL_DIR}"
    PUBLIC "${MATH_DIR}" 
    PUBLIC "${PARSERS_DIR}" 
)

target_link_directories(${PROJECT_NAME} 
    PUBLIC "${SDL_DIR}/VisualC/${CMAKE_GENERATOR_PLATFORM}/Debug"
    PUBLIC "${SDL_DIR}/VisualC/${CMAKE_GENERATOR_PLATFORM}/Release"
)

target_link_libraries(${PROJECT_NAME} 
    "SDL2.lib" 
    "SDL2main.lib"
)