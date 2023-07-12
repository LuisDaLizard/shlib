cmake_minimum_required(VERSION 3.23)
project(3d_space C)

add_executable(3d_space main.c)
target_link_libraries(3d_space shlib)
target_include_directories(3d_space PRIVATE ${SHLIB_INCLUDE})