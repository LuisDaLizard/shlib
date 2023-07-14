cmake_minimum_required(VERSION 3.23)
project(model_loading C)

add_executable(model_loading main.c)
target_link_libraries(model_loading shlib)
target_include_directories(model_loading PRIVATE ${SHLIB_INCLUDE})

file(COPY backpack DESTINATION ${CMAKE_CURRENT_BINARY_DIR})