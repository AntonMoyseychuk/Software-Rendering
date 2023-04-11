project(raytracer)

file(GLOB SRC_FILES 
    "${PROJECT_SOURCE_DIR}/app/*.cpp"
    
    "${PROJECT_SOURCE_DIR}/graphics/*.cpp"
    "${PROJECT_SOURCE_DIR}/graphics/drawables/*.cpp"
    "${PROJECT_SOURCE_DIR}/graphics/light/*.cpp"
    "${PROJECT_SOURCE_DIR}/graphics/materials/*.cpp"

    "${MATH_DIR}/src/*.cpp"
    "${WINDOW_DIR}/window/*.cpp"
    "${THREAD_POOL_DIR}/thread_pool/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${PROJECT_SOURCE_DIR}"
    
    PUBLIC "${SDL_DIR}/include"
    
    PUBLIC "${WINDOW_DIR}"
    PUBLIC "${THREAD_POOL_DIR}"
    PUBLIC "${MATH_DIR}/include" 
)

target_link_directories(${PROJECT_NAME} 
    PUBLIC "${SDL_DIR}/VisualC/${CMAKE_GENERATOR_PLATFORM}/Debug"
    PUBLIC "${SDL_DIR}/VisualC/${CMAKE_GENERATOR_PLATFORM}/Release"
)

target_link_libraries(${PROJECT_NAME} 
    "SDL2.lib" 
    "SDL2main.lib"
)