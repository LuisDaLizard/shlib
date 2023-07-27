cmake_minimum_required(VERSION 3.23)
project(shlib C)

set(SOURCE
        src/shlib_core.c
        src/shlib_math.c
        )

set(LIBS
        glfw
        glad
        )

set(INCLUDES
        libs/glad/include
        libs/glfw/include)

set(SHLIB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)

set(CMAKE_C_STANDARD 90)

add_library(shlib STATIC ${SOURCE})
target_link_libraries(shlib ${LIBS})
target_include_directories(shlib PRIVATE ${INCLUDES})

add_subdirectory(examples)