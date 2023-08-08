//
// Created by Luis Tadeo Sanchez on 7/1/23.
//


#include "shlib_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

/*********************************************************
 *                        GLOBALS                        *
 *********************************************************/

Window window = { 0 };
Input input = { 0 };

/*********************************************************
 *                    WINDOW FUNCTIONS                   *
 *********************************************************/

void window_init(int width, int height, const char *title)
{
    window.width = width;
    window.height = height;

    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    window.monitor = glfwGetPrimaryMonitor();
    window.handle = glfwCreateWindow(window.width, window.height, title, 0, 0);

    if (!window.handle)
        return;

    glfwSetWindowSizeCallback(window.handle, &window_resize_callback);
    glfwSetCursorPosCallback(window.handle, &input_mouse_pos_callback);
    glfwSetMouseButtonCallback(window.handle, &input_mouse_button_callback);

    glfwMakeContextCurrent(window.handle);

    if (!gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress))
        return;
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

Vec2 window_get_size(void)
{
    return (Vec2){(float)window.width, (float)window.height};
}

void window_disable_cursor(void)
{
    glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void window_enable_cursor(void)
{
    glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void window_resize_callback(GLFWwindow *handle, int width, int height)
{
    window.width = width;
    window.height = height;
    glViewport(0, 0, width, height);
}

/*********************************************************
 *                     INPUT FUNCTIONS                   *
 *********************************************************/

Vec2 input_get_mouse_pos()
{
    return (Vec2) {input.mouse_x, input.mouse_y};
}

bool input_is_mouse_button_down(MouseButtons button)
{
    return input.mouse_buttons[button];
}

void input_mouse_pos_callback(GLFWwindow *handle, double x, double y)
{
    input.mouse_x = (int)x;
    input.mouse_y = (int)y;
}

void input_mouse_button_callback(GLFWwindow *handle, int button, int action, int mods)
{
    input.mouse_buttons[button] = action;
}

/*********************************************************
 *                   GRAPHICS FUNCTIONS                  *
 *********************************************************/

void graphics_clear_screen(Vec4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void graphics_begin_drawing(void)
{
    glEnable(GL_DEPTH_TEST);
    window_poll_events();
}

void graphics_end_drawing(void)
{
    glDisable(GL_DEPTH_TEST);
    window_swap_buffers();
}

void graphics_enable_wireframe(void)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void graphics_disable_wireframe(void)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void graphics_enable_depth(void)
{
    glEnable(GL_DEPTH_TEST);
}

void graphics_disable_depth(void)
{
    glDisable(GL_DEPTH_TEST);
}

void graphics_enable_backface_culling(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void graphics_disable_backface_culling(void)
{
    glDisable(GL_CULL_FACE);
}

void graphics_enable_frontface_culling(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
}

void graphics_disable_frontface_culling(void)
{
    glDisable(GL_CULL_FACE);
}

void graphics_enable_alpha_blending(void)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void graphics_disable_alpha_blending(void)
{
    glDisable(GL_BLEND);
}


/*********************************************************
 *                    SHADER FUNCTIONS                   *
 *********************************************************/

Shader *shader_load(const char *vertex_src, const char *fragment_src)
{
    Shader *result = calloc(1, sizeof(Shader));

    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_src, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        printf("Vertex Shader Compilation Error: %s\n", info_log);
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_src, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        printf("Fragment Shader Compilation Error: %s\n", info_log);
    }

    result->id = glCreateProgram();
    glAttachShader(result->id, vertex);
    glAttachShader(result->id, fragment);
    glLinkProgram(result->id);

    glGetProgramiv(result->id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(result->id, 512, NULL, info_log);
        printf("Program Linking Error: %s\n", info_log);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return result;
}

void shader_unload(Shader *shader)
{
    glDeleteProgram(shader->id);
    free(shader);
}

void shader_use(Shader *shader)
{
    glUseProgram(shader->id);
}

int shader_get_location(Shader *shader, const char *name)
{
    return glGetUniformLocation(shader->id, name);
}

void shader_set_uniform_vec3(Shader *shader, int location, Vec3 value)
{
    glUseProgram(shader->id);
    glUniform3f(location, value.x, value.y, value.z);
    glUseProgram(0);
}

void shader_set_uniform_matrix(Shader *shader, int location, Matrix value)
{
    glUseProgram(shader->id);
    glUniformMatrix4fv(location, 1, GL_TRUE, (float*)&value);
    glUseProgram(0);
}

/*********************************************************
 *                   TEXTURE FUNCTIONS                   *
 *********************************************************/

Texture *texture_load(void *data, int width, int height, int channels)
{
    if (!data)
        return NULL;

    Texture *result = malloc(sizeof(Texture));

    glGenTextures(1, &result->id);
    glBindTexture(GL_TEXTURE_2D, result->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    switch(channels)
    {
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            texture_unload(result);
            return NULL;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return result;
}

void texture_unload(Texture *texture)
{
    glDeleteTextures(1, &texture->id);
    free(texture);
}

void texture_use(Texture *texture, int slot)
{
    if (!texture || slot < 0 || slot > 15)
        return;

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

/*********************************************************
 *                 FRAMEBUFFER FUNCTIONS                 *
 *********************************************************/

Framebuffer *framebuffer_create_depth(int width, int height)
{
    Framebuffer *result = malloc(sizeof(Framebuffer));
    result->texture = malloc(sizeof(Texture));
    result->texture->width = width;
    result->texture->height = height;

    glGenFramebuffers(1, &result->id);

    // Generate Depth Map
    glGenTextures(1, &result->texture->id);
    glBindTexture(GL_TEXTURE_2D, result->texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Attach depth to the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, result->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, result->texture->id, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Heyo\n");
        framebuffer_destroy(result);
        return NULL;
    }

    return result;
}

void framebuffer_destroy(Framebuffer *framebuffer)
{
    glDeleteFramebuffers(1, &framebuffer->id);
    free(framebuffer);
}

void framebuffer_bind(Framebuffer *framebuffer)
{
    glViewport(0, 0, framebuffer->texture->width, framebuffer->texture->height);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
}

void framebuffer_unbind(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window.width, window.height);
}

Texture *framebuffer_get_texture(Framebuffer *framebuffer)
{
    return framebuffer->texture;
}

/*********************************************************
 *                     MESH FUNCTIONS                    *
 *********************************************************/

Mesh *mesh_create(Vertex *vertices, unsigned int *indices, int num_vertices, int num_indices)
{
    Mesh *result = calloc(1, sizeof(Mesh));

    result->vertices = malloc(sizeof(Vertex) * num_vertices);
    result->indices = malloc(sizeof(unsigned int) * num_indices);

    memcpy(result->vertices, vertices, sizeof(Vertex) * num_vertices);
    memcpy(result->indices, indices, sizeof(unsigned int) * num_indices);

    result->num_vertices = num_vertices;
    result->num_indices = num_indices;

    mesh_setup(result);

    return result;
}

void mesh_setup(Mesh *mesh)
{
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);

    glBindVertexArray(mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, (long long)(mesh->num_vertices * sizeof(Vertex)), mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long long)(mesh->num_indices * sizeof(unsigned int)), mesh->indices, GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vec3));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vec3)));

    glBindVertexArray(0);
}

void mesh_destroy(Mesh *mesh)
{
    if (!mesh)
        return;

    if (mesh->vertices)
        free(mesh->vertices);
    if (mesh->indices)
        free(mesh->indices);

    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    glDeleteVertexArrays(1, &mesh->vao);

    free(mesh);
}

void mesh_draw(Mesh *mesh)
{
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}