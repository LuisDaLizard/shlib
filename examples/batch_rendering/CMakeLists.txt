cmake_minimum_required(VERSION 3.23)
project(batch_rendering C)

add_executable(batch_rendering main.c)
target_link_libraries(batch_rendering shlib)
target_include_directories(batch_rendering PRIVATE ${SHLIB_INCLUDE})

