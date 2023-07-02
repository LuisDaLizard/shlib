//
// Created by Luis Tadeo Sanchez on 7/1/23.
//


#include "shlib_internal.h"

void window_init(int width, int height, const char *title)
{
    window.width = width;
    window.height = height;

    if (!glfwInit())
        exit(-1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    window.handle = glfwCreateWindow(window.width, window.height, title, 0, 0);

    if (!window.handle)
        exit(-1);

    glfwMakeContextCurrent(window.handle);

    if (!gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress))
        exit(-1);
}

void window_destroy(void)
{
    glfwDestroyWindow(window.handle);
    glfwTerminate();
}

bool window_should_close(void)
{
    return glfwWindowShouldClose(window.handle);
}

void window_poll_events(void)
{
    glfwPollEvents();
}

void window_swap_buffers(void)
{
    glfwSwapBuffers(window.handle);
}