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
    glfwWindowHint(GLFW_SAMPLES, 8);

    window.monitor = glfwGetPrimaryMonitor();
    window.handle = glfwCreateWindow(window.width, window.height, title, 0, 0);

    if (!window.handle)
        exit(-1);

    glfwSetWindowSizeCallback(window.handle, &window_resize_callback);

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

Vec2 window_get_size(void)
{
    return (Vec2){(float)window.width, (float)window.height};
}

void window_resize_callback(GLFWwindow *handle, int width, int height)
{
    window.width = width;
    window.height = height;
    glViewport(0, 0, width, height);
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
    glBufferData(GL_ARRAY_BUFFER, mesh->num_vertices * sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->num_indices * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

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
    mesh = 0;
}

/*********************************************************
 *                    MODEL FUNCTIONS                    *
 *********************************************************/

Model *model_load_from_mesh(Mesh *mesh)
{
    Model *result = malloc(sizeof(Model));

    result->meshes = malloc(sizeof(Mesh));
    result->meshes[0] = *mesh;
    result->num_meshes = 1;

    return result;
}

Model *model_load_from_file(const char *path)
{
    Model *result = malloc(sizeof(Model));

    return result;
}

void model_draw(Model *model)
{
    int i;
    for (i = 0; i < model->num_meshes; i++)
    {
        glBindVertexArray(model->meshes[i].vao);
        glDrawElements(GL_TRIANGLES, model->meshes[i].num_indices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void model_unload(Model *model)
{
    if (!model)
        return;

    if (model->meshes)
    {
        int i;
        for (i = 0; i < model->num_meshes; i++)
            mesh_destroy(&model->meshes[i]);
        free(model->meshes);
    }

    free(model);
    model = 0;
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
