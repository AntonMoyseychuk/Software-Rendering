project(raytracer)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES 
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_UTIL_DIR}/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${PROJECT_SOURCE_DIR}"
        
    PUBLIC "${PROJECT_DEPENDENCIES_DIR}"
    PUBLIC "${PROJECT_DEPENDENCIES_DIR}/SDL/include"

    PUBLIC "${PROJECT_UTIL_DIR}/thread_pool"
    PUBLIC "${PROJECT_UTIL_DIR}/own-math-3d"
    PUBLIC "${PROJECT_UTIL_DIR}/window"
)

target_link_directories(${PROJECT_NAME} 
    PUBLIC "${PROJECT_DEPENDENCIES_DIR}/SDL/VisualC/${CMAKE_GENERATOR_PLATFORM}/Debug"
    PUBLIC "${PROJECT_DEPENDENCIES_DIR}/SDL/VisualC/${CMAKE_GENERATOR_PLATFORM}/Release"
)

target_link_libraries(${PROJECT_NAME} 
    "SDL2.lib" 
    "SDL2main.lib"
)