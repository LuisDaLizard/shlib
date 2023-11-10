//
// Created by Luis Tadeo Sanchez on 7/1/23.
//


#include "shlib_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

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

    glfwSetFramebufferSizeCallback(window.handle, &window_resize_callback);
    glfwSetCursorPosCallback(window.handle, &input_mouse_pos_callback);
    glfwSetMouseButtonCallback(window.handle, &input_mouse_button_callback);
    glfwSetKeyCallback(window.handle, &input_key_callback);

    glfwMakeContextCurrent(window.handle);

    if (!gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress))
        return;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

bool input_is_key_down(Keys key)
{
    return input.keys[key];
}

double input_get_time()
{
    return glfwGetTime();
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

void input_key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods)
{
    input.keys[key] = action;
}

/*********************************************************
 *                   GRAPHICS FUNCTIONS                  *
 *********************************************************/

void graphics_clear_screen(Vec4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void graphics_draw_batch(Batch *batch)
{
    if (!batch->num_quads)
        return;

    int i;
    for (i = 0; i < batch->num_textures; i++)
        texture_use(batch->textures[i], i);

    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (long)(batch->num_quads * 4 * sizeof(Vertex2D)), batch->vertices);

    glBindVertexArray(batch->vao);
    glDrawElements(GL_TRIANGLES, (int)(batch->num_quads * 6), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    batch->num_quads = 0;
    batch->num_textures = 1;
}

void graphics_draw_mesh(Mesh *mesh)
{
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, (int)mesh->num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

/*********************************************************
 *                    SHADER FUNCTIONS                   *
 *********************************************************/

Shader *shader_load_from_file(const char *vert_path, const char *frag_path)
{
    char *vert_src = utils_read_file(vert_path);
    char *frag_src = utils_read_file(frag_path);

    if (!vert_src || !frag_src)
        return 0;

    Shader *shader = shader_load(vert_src, frag_src);

    free(vert_src);
    free(frag_src);

    return shader;
}

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

void shader_upload_int(Shader *shader, const char *name, int value)
{
    glUseProgram(shader->id);
    int location = glGetUniformLocation(shader->id, name);

    if (location == -1)
        return;

    glUniform1i(location, value);
}

void shader_upload_int_array(Shader *shader, const char *name, int n, int *value)
{
    glUseProgram(shader->id);
    int location = glGetUniformLocation(shader->id, name);

    if (location == -1)
        return;

    glUniform1iv(location, n, value);
}

void shader_upload_float(Shader *shader, const char *name, float value)
{
    glUseProgram(shader->id);
    int location = glGetUniformLocation(shader->id, name);

    if (location == -1)
        return;

    glUniform1f(location, value);
}

void shader_upload_vec2(Shader *shader, const char *name, Vec2 value)
{
    glUseProgram(shader->id);
    int location = glGetUniformLocation(shader->id, name);

    if (location == -1)
        return;

    glUniform2f(location, value.x, value.y);
}

void shader_upload_vec3(Shader *shader, const char *name, Vec3 value)
{
    glUseProgram(shader->id);
    int location = glGetUniformLocation(shader->id, name);

    if (location == -1)
        return;

    glUniform3f(location, value.x, value.y, value.z);
}

void shader_upload_vec4(Shader *shader, const char *name, Vec4 value)
{
    glUseProgram(shader->id);
    int location = glGetUniformLocation(shader->id, name);

    if (location == -1)
        return;

    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void shader_upload_matrix(Shader *shader, const char *name, Matrix value)
{
    glUseProgram(shader->id);
    int location = glGetUniformLocation(shader->id, name);

    if (location == -1)
        return;

    glUniformMatrix4fv(location, 1, GL_TRUE, (float*)&value);
}

/*********************************************************
 *                   TEXTURE FUNCTIONS                   *
 *********************************************************/

Texture *texture_load_from_file(const char *path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
    if (!data)
        return 0;

    Texture *texture = texture_load(data, width, height, channels);

    stbi_image_free(data);

    return texture;
}

Texture *texture_load(void *data, int width, int height, int channels)
{
    if (!data)
        return NULL;

    Texture *result = malloc(sizeof(Texture));
    result->width = width;
    result->height = height;
    result->channels = channels;

    glGenTextures(1, &result->id);
    glBindTexture(GL_TEXTURE_2D, result->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch(channels)
    {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
            break;
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
 *                     BATCH FUNCTIONS                   *
 *********************************************************/

Batch *batch_create(unsigned int max_quads)
{
    Batch *batch = malloc(sizeof(Batch));

    batch->max_quads = max_quads;
    batch->num_quads = 0;
    batch->num_textures = 1;

    batch->vertices = malloc(max_quads * 4 * sizeof(Vertex2D));
    batch->indices = malloc(max_quads * 6 * sizeof(unsigned int));
    batch->textures = malloc(16 * sizeof(Texture *));
    unsigned char white[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
    batch->textures[0] = texture_load(white, 1, 1, 4);

    // Generate indices
    int i, index = 0;
    for (i = 0; i < max_quads * 6; i += 6)
    {
        batch->indices[i + 0] = index + 0;
        batch->indices[i + 1] = index + 1;
        batch->indices[i + 2] = index + 2;

        batch->indices[i + 3] = index + 2;
        batch->indices[i + 4] = index + 3;
        batch->indices[i + 5] = index + 0;

        index += 4;
    }

    glGenVertexArrays(1, &batch->vao);
    glGenBuffers(1, &batch->vbo);
    glGenBuffers(1, &batch->ebo);

    glBindVertexArray(batch->vao);

    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
    glBufferData(GL_ARRAY_BUFFER, (long)(batch->max_quads * 4 * sizeof(Vertex2D)), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)sizeof(Vec3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)(sizeof(Vec3) + sizeof(Vec4)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)(sizeof(Vec4) + sizeof(Vec3) + sizeof(Vec2)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long)(batch->max_quads * 6 * sizeof(unsigned int)), batch->indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    return batch;
}

void batch_destroy(Batch *batch)
{
    glDeleteBuffers(1, &batch->vbo);
    glDeleteBuffers(1, &batch->ebo);
    glDeleteVertexArrays(1, &batch->vao);

    texture_unload(batch->textures[0]);

    free(batch->textures);
    free(batch->vertices);
    free(batch->indices);
    free(batch);
}

void batch_add_sprite(Batch *batch, Vec2 position, Vec2 size, Texture *texture)
{
    if (batch->num_quads >= batch->max_quads)
        return;

    int i;
    for (i = 1; i < batch->num_textures; i++)
    {
        if (batch->textures[i]->id == texture->id)
            break;
    }

    if (i >= 15)
        return;

    if (i >= batch->num_textures)
    {
        batch->textures[i] = texture;
        batch->num_textures++;
    }

    unsigned int vertex_index = batch->num_quads * 4;

    float left = position.x - (size.x / 2.0f);
    float right = position.x + (size.x / 2.0f);
    float top = position.y + (size.y / 2.0f);
    float bottom = position.y - (size.y / 2.0f);

    batch->vertices[vertex_index + 0] = (Vertex2D){{left, top, 0}, {1, 1, 1, 1}, {0, 1}, (float)i};
    batch->vertices[vertex_index + 1] = (Vertex2D){{right, top, 0}, {1, 1, 1, 1}, {1, 1}, (float)i};
    batch->vertices[vertex_index + 2] = (Vertex2D){{right, bottom, 0}, {1, 1, 1, 1}, {1, 0}, (float)i};
    batch->vertices[vertex_index + 3] = (Vertex2D){{left, bottom, 0}, {1, 1, 1, 1}, {0, 0}, (float)i};

    batch->num_quads++;
}

void batch_add_sprite_uv(Batch *batch, Vec2 position, Vec2 size, Vec2 uv[4], Texture *texture)
{
    if (batch->num_quads >= batch->max_quads)
        return;

    int i;
    for (i = 1; i < batch->num_textures; i++)
    {
        if (batch->textures[i]->id == texture->id)
            break;
    }

    if (i >= 15)
        return;

    if (i >= batch->num_textures)
    {
        batch->textures[i] = texture;
        batch->num_textures++;
    }

    unsigned int vertex_index = batch->num_quads * 4;

    float left = position.x - (size.x / 2.0f);
    float right = position.x + (size.x / 2.0f);
    float top = position.y + (size.y / 2.0f);
    float bottom = position.y - (size.y / 2.0f);

    batch->vertices[vertex_index + 0] = (Vertex2D){{left, top, 0}, {1, 1, 1, 1}, uv[0], (float)i};
    batch->vertices[vertex_index + 1] = (Vertex2D){{right, top, 0}, {1, 1, 1, 1}, uv[1], (float)i};
    batch->vertices[vertex_index + 2] = (Vertex2D){{right, bottom, 0}, {1, 1, 1, 1}, uv[2], (float)i};
    batch->vertices[vertex_index + 3] = (Vertex2D){{left, bottom, 0}, {1, 1, 1, 1}, uv[3], (float)i};

    batch->num_quads++;
}

void batch_add_text(Batch *batch, Vec2 position, Font *font, const char *text)
{
    float x = position.x, y = position.y;
    int i = 0;
    while(text[i])
    {
        if (text[i] < 32)
        {
            i++;
            continue;
        }

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad((stbtt_bakedchar *)font->character_data, font->bitmap->width, font->bitmap->height, text[i] - 32, &x, &y, &q, 1);

        Character c = font->character_data[text[i] - 32];

        Vec2 uv[4];
        uv[0] = (Vec2){q.s0, q.t0};
        uv[1] = (Vec2){q.s1, q.t0};
        uv[2] = (Vec2){q.s1, q.t1};
        uv[3] = (Vec2){q.s0, q.t1};

        float left = x - c.xadvance + q.x0;
        float top = y + q.y0;
        float right = x - c.xadvance + q.x1;
        float bottom = y + q.y1;

        float width = right - left;
        float height = bottom - top;

        batch_add_sprite_uv(batch, (Vec2){x - (c.xadvance / 2.0f), y - (q.y1 / 2.0f) - (top / 2.0f)}, (Vec2){width, height}, uv, font->bitmap);
        i++;
    }
}

void batch_add_quad(Batch *batch, Vec2 position, Vec2 size, Vec4 color)
{
    if (batch->num_quads >= batch->max_quads)
        return;

    unsigned int vertex_index = batch->num_quads * 4;

    float left = position.x - (size.x / 2.0f);
    float right = position.x + (size.x / 2.0f);
    float top = position.y + (size.y / 2.0f);
    float bottom = position.y - (size.y / 2.0f);

    batch->vertices[vertex_index + 0] = (Vertex2D){{left, top, 0}, color, {0, 1}, 0};
    batch->vertices[vertex_index + 1] = (Vertex2D){{right, top, 0}, color, {1, 1}, 0};
    batch->vertices[vertex_index + 2] = (Vertex2D){{right, bottom, 0}, color, {1, 0}, 0};
    batch->vertices[vertex_index + 3] = (Vertex2D){{left, bottom, 0}, color, {0, 0}, 0};

    batch->num_quads++;
}

/*********************************************************
 *                     FONT FUNCTIONS                    *
 *********************************************************/

Font *font_load_from_file(const char *path, float font_size)
{
    unsigned char *bytes = utils_read_file_bytes(path);

    if (!bytes)
        return 0;

    // TODO: find automatic way to determine resolution
    int w_res = 2048;
    int h_res = 2048;

    unsigned char *temp_bitmap = malloc(w_res * h_res);

    Font *font = malloc(sizeof(Font));

    stbtt_BakeFontBitmap(bytes, 0, font_size, temp_bitmap, w_res, h_res, 32, 96, (stbtt_bakedchar *)font->character_data);
    font->bitmap = texture_load(temp_bitmap, w_res, h_res, 1);

    free(temp_bitmap);
    free(bytes);
    return font;
}

void font_unload(Font *font)
{
    texture_unload(font->bitmap);
    free(font);
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

Mesh *mesh_create(Vertex3D *vertices, unsigned int *indices, int num_vertices, int num_indices)
{
    Mesh *result = calloc(1, sizeof(Mesh));

    result->vertices = malloc(sizeof(Vertex3D) * num_vertices);
    result->indices = malloc(sizeof(unsigned int) * num_indices);

    memcpy(result->vertices, vertices, sizeof(Vertex3D) * num_vertices);
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
    glBufferData(GL_ARRAY_BUFFER, (long long)(mesh->num_vertices * sizeof(Vertex3D)), mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long long)(mesh->num_indices * sizeof(unsigned int)), mesh->indices, GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)sizeof(Vec3));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)(2 * sizeof(Vec3)));

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