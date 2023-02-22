set(LIB_NAME "window_framework_")

project("${LIB_NAME}${CMAKE_GENERATOR_PLATFORM}")

file(GLOB LIB_SRC "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/include/window_framework/*.hpp")

add_library(${PROJECT_NAME} STATIC ${LIB_SRC})

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${MATH_3D_DIR}/include" 
    PUBLIC "${SDL_DIR}/include"
    PUBLIC "${PROJECT_SOURCE_DIR}/include/window_framework"
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