project(raytracer)

file(GLOB SRC_FILES 
    "${PROJECT_SOURCE_DIR}/src/*.cpp" 
    "${PROJECT_SOURCE_DIR}/src/*.hpp"

    "${MATH_3D_DIR}/src/*.cpp" 
    "${MATH_3D_DIR}/include/math_3d/*.hpp"

    "${WINDOW_FRAMEWORK_DIR}/src/*.cpp" 
    "${WINDOW_FRAMEWORK_DIR}/include/window_framework/*.hpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${MATH_3D_DIR}/include" 
    PUBLIC "${SDL_DIR}/include"
    PUBLIC "${WINDOW_FRAMEWORK_DIR}/include"
)

target_link_directories(${PROJECT_NAME} 
    PUBLIC "${SDL_DIR}/lib/${CMAKE_GENERATOR_PLATFORM}"
)

target_link_libraries(${PROJECT_NAME} 
    "SDL2.lib" 
    "SDL2main.lib"
)