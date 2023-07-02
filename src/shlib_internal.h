//
// Created by Luis Tadeo Sanchez on 7/1/23.
//

#ifndef SHLIB_SHLIB_INTERNAL_H
#define SHLIB_SHLIB_INTERNAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    GLFWwindow *handle;
    int width;
    int height;
} Window;

Window window;

#endif //SHLIB_SHLIB_INTERNAL_H
