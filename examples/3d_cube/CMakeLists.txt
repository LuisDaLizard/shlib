cmake_minimum_required(VERSION 3.23)
project(3d_cube C)

add_executable(3d_cube main.c)
target_link_libraries(3d_cube shlib)
target_include_directories(3d_cube PRIVATE ${SHLIB_INCLUDE})