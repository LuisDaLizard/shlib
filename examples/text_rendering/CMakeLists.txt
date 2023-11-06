cmake_minimum_required(VERSION 3.23)
project(text_rendering C)

add_executable(text_rendering main.c)
target_link_libraries(text_rendering shlib)
target_include_directories(text_rendering PRIVATE ${SHLIB_INCLUDE})

file(COPY retro.ttf DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)