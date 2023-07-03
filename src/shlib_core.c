//
// Created by Luis Tadeo Sanchez on 7/1/23.
//


#include "shlib_internal.h"

Window window = { 0 };
Graphics graphics = { 0 };

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

    window.monitor = glfwGetPrimaryMonitor();
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

void window_toggle_fullscreen(void)
{
    if (!window.fullscreen)
    {
        const GLFWvidmode *video_mode = glfwGetVideoMode(window.monitor);
        glfwSetWindowMonitor(window.handle, window.monitor, 0, 0, video_mode->width, video_mode->height, 0);
        window.fullscreen = true;
    }
    else
    {
        glfwSetWindowMonitor(window.handle, 0, 0, 0, window.width, window.height, 0);
        window.fullscreen = false;
    }
}

void graphics_clear_screen(float color[4])
{
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void graphics_begin_drawing(void)
{
    window_poll_events();
    graphics.projection = matrix_ortho(-1, 1, -1, 1, 0.01f, 1000.0f);
}

void graphics_end_drawing(void)
{
    window_swap_buffers();
}

void graphics_draw_quad(void)
{

}

Batch batch_create(int max_quads)
{
    Batch result = { 0 };
    result.max_quads = max_quads;

    result.vertices = malloc(sizeof(Vertex) * max_quads * 4);
    result.indices = malloc(sizeof(unsigned int) * max_quads * 6);

    int i, j = 0;
    for (i = 0; i < max_quads * 6; i += 6)
    {
        result.indices[i + 0] = j + 0;
        result.indices[i + 1] = j + 1;
        result.indices[i + 2] = j + 2;

        result.indices[i + 3] = j + 2;
        result.indices[i + 4] = j + 3;
        result.indices[i + 5] = j + 0;

        j += 4;
    }

    glGenVertexArrays(1, &result.vao);
    glBindVertexArray(result.vao);

    glGenBuffers(1, &result.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, result.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * max_quads * 4, 0, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vec3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vec3) + sizeof(Vec3)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &result.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * max_quads * 6, result.indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    return result;
}
