cmake_minimum_required(VERSION 3.23)
project(basic_window C)

add_executable(blank_window main.c)
target_link_libraries(blank_window shlib)
target_include_directories(blank_window PRIVATE ${SHLIB_INCLUDE})

