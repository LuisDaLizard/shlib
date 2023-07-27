cmake_minimum_required(VERSION 3.23)
project(depth_buffer C)

add_executable(depth_buffer main.c)
target_link_libraries(depth_buffer shlib)
target_include_directories(depth_buffer PRIVATE ${SHLIB_INCLUDE})