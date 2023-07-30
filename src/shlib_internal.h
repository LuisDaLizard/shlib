//
// Created by Luis Tadeo Sanchez on 7/1/23.
//

#ifndef SHLIB_SHLIB_INTERNAL_H
#define SHLIB_SHLIB_INTERNAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>

/*********************************************************
 *                       STRUCTURES                      *
 *********************************************************/

typedef struct
{
    float x, y;
} Vec2;

typedef struct
{
    float x, y, z;
} Vec3;

typedef struct
{
    float x, y, z, w;
} Vec4;

typedef struct
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
} Matrix;

typedef struct
{
    Vec3 position;
    Vec3 normal;
    Vec2 tex_coord;
} Vertex;

typedef struct
{
    unsigned int id;
} Shader;

typedef struct
{
    int width, height, channels;
    unsigned int id;
} Texture;

typedef struct
{
    unsigned int id;
    Texture *texture;
} Framebuffer;

typedef struct
{
    GLFWwindow *handle;
    GLFWmonitor *monitor;
    int width;
    int height;
    bool fullscreen;
} Window;

typedef struct
{
    int mouse_x, mouse_y;
} Input;

typedef struct
{
    Vertex *vertices;
    unsigned int *indices;

    int num_vertices;
    int num_indices;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
} Mesh;

/*********************************************************
 *                    WINDOW FUNCTIONS                   *
 *********************************************************/

void window_init(int width, int height, const char *title);
void window_destroy(void);
bool window_should_close(void);
void window_poll_events(void);
void window_swap_buffers(void);
void window_toggle_fullscreen(void);
Vec2 window_get_size(void);
void window_disable_cursor(void);
void window_enable_cursor(void);

void window_resize_callback(GLFWwindow *handle, int width, int height);

/*********************************************************
 *                     INPUT FUNCTIONS                   *
 *********************************************************/

Vec2 input_get_mouse_pos(void);

void input_mouse_pos_callback(GLFWwindow *handle, double x, double y);

/*********************************************************
 *                   GRAPHICS FUNCTIONS                  *
 *********************************************************/

void graphics_clear_screen(Vec4 color);
void graphics_begin_drawing(void);
void graphics_end_drawing(void);
void graphics_enable_wireframe(void);
void graphics_disable_wireframe(void);
void graphics_enable_depth(void);
void graphics_disable_depth(void);
void graphics_enable_backface_culling(void);
void graphics_disable_backface_culling(void);
void graphics_enable_frontface_culling(void);
void graphics_disable_frontface_culling(void);
void graphics_enable_alpha_blending(void);
void graphics_disable_alpha_blending(void);

/*********************************************************
 *                    SHADER FUNCTIONS                   *
 *********************************************************/

Shader *shader_load(const char *vertex_src, const char *fragment_src);
void shader_unload(Shader *shader);
void shader_use(Shader *shader);
int shader_get_location(Shader *shader, const char *name);
void shader_set_uniform_vec3(Shader *shader, int location, Vec3 value);
void shader_set_uniform_matrix(Shader *shader, int location, Matrix value);

/*********************************************************
 *                   TEXTURE FUNCTIONS                   *
 *********************************************************/

Texture *texture_load(void *data, int width, int height, int channels);
void texture_unload(Texture *texture);
void texture_use(Texture *texture, int slot);

/*********************************************************
 *                 FRAMEBUFFER FUNCTIONS                 *
 *********************************************************/

Framebuffer *framebuffer_create_depth(int width, int height);
void framebuffer_destroy(Framebuffer *framebuffer);
void framebuffer_bind(Framebuffer *framebuffer);
void framebuffer_unbind(void);
Texture *framebuffer_get_texture(Framebuffer *framebuffer);

/*********************************************************
 *                     MESH FUNCTIONS                    *
 *********************************************************/

Mesh *mesh_create(Vertex *vertices, unsigned int *indices, int num_vertices, int num_indices);
void mesh_destroy(Mesh *mesh);
void mesh_draw(Mesh *mesh);

void mesh_setup(Mesh *mesh);

/*********************************************************
 *                  CORE MATH FUNCTIONS                  *
 *********************************************************/

Matrix matrix_identity(void);
Vec3 matrix_mul_vec3(Matrix left, Vec3 right);
Matrix matrix_mul(Matrix left, Matrix right);
void matrix_print(Matrix matrix);
void vec3_print(Vec3 vector);

/*********************************************************
 *            VECTOR TRANSFORMATION FUNCTIONS            *
 *********************************************************/

Vec3 vec3_add(Vec3 left, Vec3 right);
Vec3 vec3_sub(Vec3 left, Vec3 right);
float vec3_dot(Vec3 left, Vec3 right);
Vec3 vec3_normalize(Vec3 vector);
Vec3 vec3_cross(Vec3 left, Vec3 right);
Vec3 vec3_negate(Vec3 vector);

/*********************************************************
 *            MATRIX TRANSFORMATION FUNCTIONS            *
 *********************************************************/

Matrix matrix_create_scalar(Vec3 scalar);
Matrix matrix_create_rotation(Vec3 axis, float degrees);
Matrix matrix_create_translation(Vec3 translation);
Matrix matrix_scale(Matrix matrix, Vec3 scalar);
Matrix matrix_rotate(Matrix matrix, Vec3 axis, float degrees);
Matrix matrix_translate(Matrix matrix, Vec3 translation);

/*********************************************************
 *           MATRIX PROJECTION & VIEW FUNCTIONS          *
 *********************************************************/

Matrix matrix_ortho(float left, float right, float top, float bottom, float near, float far);
Matrix matrix_perspective(float aspect, float fov, float near, float far);
Matrix matrix_look_at(Vec3 eye, Vec3 target, Vec3 up);


#endif //SHLIB_SHLIB_INTERNAL_H
