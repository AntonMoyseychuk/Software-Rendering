set(CMAKE_GENERATOR_PLATFORM win32)
message("[user info] CMAKE_GENERATOR_PLATFORM = ${CMAKE_GENERATOR_PLATFORM}")

project(rasterizer)

set(MATH_SRC_DIR "${PROJECT_SOURCE_DIR}/..")
#set(SDL_INCLUDE_DIR "${DEPENDENCIES_DIRECTORY}/SDL/include")

file(GLOB SRC_FILES 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${MATH_SRC_DIR}/math/*.hpp" "${MATH_SRC_DIR}/math/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${MATH_SRC_DIR} 
    #PUBLIC ${SDL_INCLUDE_DIR}
)