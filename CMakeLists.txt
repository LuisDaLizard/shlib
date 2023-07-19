cmake_minimum_required(VERSION 3.23)
project(shlib C)

set(CMAKE_C_STANDARD 23)

set(SOURCE
        src/shlib_core.c
        src/shlib_math.c
        src/shlib_memory.c
        )

set(LIBS
        assimp
        glfw
        glad
        )

set(INCLUDES
        libs/glad/include
        libs/glfw/include
        libs/assimp/include)

set(SHLIB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(libs/glfw)
add_subdirectory(libs/assimp)
add_subdirectory(libs/glad)

add_library(shlib STATIC ${SOURCE})
target_link_libraries(shlib ${LIBS})
target_include_directories(shlib PRIVATE ${INCLUDES})

add_subdirectory(examples)