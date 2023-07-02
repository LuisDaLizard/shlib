cmake_minimum_required(VERSION 3.23)
project(shlib C)

set(CMAKE_C_STANDARD 90)

add_subdirectory(libs/glfw)

set(SOURCE
        src/glad/src/glad.c
        src/shlib_core.c
        )

add_library(shlib ${SOURCE})
target_link_libraries(shlib glfw)
target_include_directories(shlib PRIVATE src/glad/include libs/glfw/include)