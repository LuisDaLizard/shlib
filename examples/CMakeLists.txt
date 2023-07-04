cmake_minimum_required(VERSION 3.23)
project(shlib_examples C)

set(BLANKWINDOW_SRC blank_window/main.c)



add_executable(blank_window ${BLANKWINDOW_SRC})
target_link_libraries(blank_window shlib)
target_include_directories(blank_window PRIVATE ../include/)