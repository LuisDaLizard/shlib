cmake_minimum_required(VERSION 3.23)
project(first_quad C)

add_executable(first_quad main.c)
target_link_libraries(first_quad shlib)
target_include_directories(first_quad PRIVATE ${SHLIB_INCLUDE})
