//
// Created by Luis Tadeo Sanchez on 7/1/23.
//


#include "shlib_internal.h"

/*********************************************************
 *                        GLOBALS                        *
 *********************************************************/

Window window = { 0 };
Graphics graphics = { 0 };

/*********************************************************
 *                    WINDOW FUNCTIONS                   *
 *********************************************************/

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

    const char *vertex_src = "#version 400 core\n"
                             "\n"
                             "layout (location = 0) in vec3 aPosition;\n"
                             "layout (location = 1) in vec3 aNormal;\n"
                             "layout (location = 2) in vec2 aTexCoord;\n"
                             "\n"
                             "void main()\n"
                             "{\n"
                             "    gl_Position = vec4(aPosition, 1);\n"
                             "}";
    const char *fragment_src = "#version 400 core\n"
                               "\n"
                               "out vec4 oColor;\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    oColor = vec4(1, 0, 1, 1);\n"
                               "}";

    graphics.base_shader = shader_load_from_memory(vertex_src, fragment_src);
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
    window_poll_events();
    graphics.projection = matrix_ortho(-1, 1, -1, 1, 0.01f, 1000.0f);
    graphics.current_batch = batch_create(1000);
}

void graphics_end_drawing(void)
{
    batch_flush(graphics.current_batch);
    window_swap_buffers();
}

void graphics_draw_demo(void)
{

}

/*********************************************************
 *                    SHADER FUNCTIONS                   *
 *********************************************************/

Shader *shader_load_from_file(const char *vertex_path, const char *fragment_path)
{
    Shader *result = calloc(1, sizeof(Shader));
    char *vertex_src, *fragment_src;

    vertex_src = file_read(vertex_path);
    fragment_src = file_read(fragment_path);

    if (!vertex_src)
        printf("Unable to read vertex shader file.\n");

    if (!vertex_src)
        printf("Unable to read fragment shader file.\n");

    if (!vertex_src || !fragment_src)
        return result;

    result = shader_load_from_memory(vertex_src, fragment_src);

    free(vertex_src);
    free(fragment_src);

    return result;
}

Shader *shader_load_from_memory(const char *vertex_src, const char *fragment_src)
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

/*********************************************************
 *                     BATCH FUNCTIONS                   *
 *********************************************************/

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

void batch_flush(Batch batch)
{
    if (batch.num_triangles == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, batch.num_vertices * sizeof(Vertex), batch.vertices);

    glUseProgram(graphics.base_shader->id);
    // upload uniforms
    glBindVertexArray(batch.vao);
    glDrawElements(GL_TRIANGLES, batch.num_triangles, GL_UNSIGNED_INT, (void *)0);
    glBindVertexArray(0);

    free(batch.vertices);
    free(batch.indices);
}

/*********************************************************
 *                 FILE UTILITY FUNCTIONS                *
 *********************************************************/

char *file_read(const char *path)
{
    FILE *file = fopen(path, "r");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *result = malloc(length + 1);
    unsigned int result_len = fread(result, length, 1, file);
    result[length] = 0;

    fclose(file);

    if (result_len != length)
        return NULL;

    return result;
}
