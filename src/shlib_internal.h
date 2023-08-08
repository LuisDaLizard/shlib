//
// Created by Luis Tadeo Sanchez on 7/1/23.
//

#ifndef SHLIB_SHLIB_INTERNAL_H
#define SHLIB_SHLIB_INTERNAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>

/*********************************************************
 *                      ENUMERATIONS                     *
 *********************************************************/

typedef enum
{
    MOUSE_LEFT = 0,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_COUNT = 3,
} MouseButtons;

typedef enum
{
    KEY_SPACE = 32,
    KEY_ESC = 256,
    KEY_ENTER,
    KEY_TAB,
    KEY_BACKSPACE,
    KEY_INSERT,
    KEY_DELETE,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    KEY_A = 65,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_COUNT = 300,
} Keys;

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

    bool mouse_buttons[MOUSE_COUNT];
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
bool input_is_mouse_button_down(MouseButtons button);

void input_mouse_pos_callback(GLFWwindow *handle, double x, double y);
void input_mouse_button_callback(GLFWwindow *handle, int button, int action, int mods);

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
