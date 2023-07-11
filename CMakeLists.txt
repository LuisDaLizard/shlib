cmake_minimum_required(VERSION 3.23)
project(shlib C)

set(CMAKE_C_STANDARD 90)

option(BUILD_EXAMPLES "build example applications with the library" ON)

add_subdirectory(libs/glfw)

set(SOURCE
        src/glad/src/glad.c
        src/shlib_core.c
        src/shlib_math.c
        )

set(SHLIB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(shlib ${SOURCE})
target_link_libraries(shlib glfw)
target_include_directories(shlib PRIVATE src/glad/include libs/glfw/include)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()