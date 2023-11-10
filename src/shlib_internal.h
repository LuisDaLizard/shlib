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

typedef enum MouseButtons
{
    MOUSE_LEFT = 0,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_COUNT = 3,
} MouseButtons;

typedef enum Keys
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

typedef struct Vec2
{
    float x, y;
} Vec2;

typedef struct Vec3
{
    float x, y, z;
} Vec3;

typedef struct Vec4
{
    float x, y, z, w;
} Vec4;

typedef struct Vec4 Quaternion;

typedef struct Matrix
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
} Matrix;


typedef struct Vertex2D
{
    Vec3 position;
    Vec4 color;
    Vec2 tex_coord;
    float tex_id;
} Vertex2D;

typedef struct Vertex3D
{
    Vec3 position;
    Vec3 normal;
    Vec2 tex_coord;
} Vertex3D;

typedef struct Shader
{
    unsigned int id;
} Shader;

typedef struct Texture
{
    unsigned int width;
    unsigned int height;
    unsigned int channels;

    unsigned int id;
} Texture;

typedef struct Mesh
{
    Vertex3D *vertices;
    unsigned int *indices;

    unsigned int num_vertices;
    unsigned int num_indices;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
} Mesh;

typedef struct Framebuffer
{
    unsigned int id;
    Texture *texture;
} Framebuffer;

typedef struct Character
{
    unsigned short x0, y0, x1, y1;
    float xoff, yoff, xadvance;
} Character;

typedef struct Font
{
    Texture *bitmap;
    Character character_data[96];
} Font;

typedef struct Batch
{
    unsigned int max_quads;
    unsigned int num_quads;

    Vertex2D *vertices;
    unsigned int *indices;

    Texture **textures;
    unsigned int num_textures;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
} Batch;

typedef struct Window
{
    GLFWwindow *handle;
    GLFWmonitor *monitor;
    int width;
    int height;
    bool fullscreen;
} Window;

typedef struct Input
{
    int mouse_x, mouse_y;

    bool mouse_buttons[MOUSE_COUNT];
    bool keys[KEY_COUNT];
} Input;


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
 *                     UTIL FUNCTIONS                    *
 *********************************************************/

char *utils_read_file(const char *path);
unsigned char *utils_read_file_bytes(const char *path);

/*********************************************************
 *                     INPUT FUNCTIONS                   *
 *********************************************************/

Vec2 input_get_mouse_pos(void);
bool input_is_mouse_button_down(MouseButtons button);
bool input_is_key_down(Keys key);
double input_get_time();

void input_mouse_pos_callback(GLFWwindow *handle, double x, double y);
void input_mouse_button_callback(GLFWwindow *handle, int button, int action, int mods);
void input_key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods);

/*********************************************************
 *                   GRAPHICS FUNCTIONS                  *
 *********************************************************/

void graphics_clear_screen(Vec4 color);
void graphics_draw_batch(Batch *batch);
void graphics_draw_mesh(Mesh *mesh);

/*********************************************************
 *                    SHADER FUNCTIONS                   *
 *********************************************************/

Shader *shader_load_from_file(const char *vert_path, const char *frag_path);
Shader *shader_load(const char *vertex_src, const char *fragment_src);
void shader_unload(Shader *shader);
void shader_use(Shader *shader);
void shader_upload_int(Shader *shader, const char *name, int value);
void shader_upload_int_array(Shader *shader, const char *name, int n, int *value);
void shader_upload_float(Shader *shader, const char *name, float value);
void shader_upload_vec2(Shader *shader, const char *name, Vec2 value);
void shader_upload_vec3(Shader *shader, const char *name, Vec3 value);
void shader_upload_vec4(Shader *shader, const char *name, Vec4 value);
void shader_upload_matrix(Shader *shader, const char *name, Matrix value);

/*********************************************************
 *                   TEXTURE FUNCTIONS                   *
 *********************************************************/

Texture *texture_load_from_file(const char *path);
Texture *texture_load(void *data, int width, int height, int channels);
void texture_unload(Texture *texture);
void texture_use(Texture *texture, int slot);

/*********************************************************
 *                     BATCH FUNCTIONS                   *
 *********************************************************/

Batch *batch_create(unsigned int max_quads);
void batch_destroy(Batch *batch);
void batch_add_sprite(Batch *batch, Vec2 position, Vec2 size, Texture *texture);
void batch_add_sprite_uv(Batch *batch, Vec2 position, Vec2 size, Vec2 uv[4], Texture *texture);
void batch_add_text(Batch *batch, Vec2 position, Font *font, const char *text);
void batch_add_quad(Batch *batch, Vec2 position, Vec2 size, Vec4 color);

/*********************************************************
 *                     FONT FUNCTIONS                    *
 *********************************************************/

Font *font_load_from_file(const char *path, float font_size);
void font_unload(Font *font);

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

Mesh *mesh_create(Vertex3D *vertices, unsigned int *indices, int num_vertices, int num_indices);
void mesh_destroy(Mesh *mesh);

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
Vec3 vec3_scale(Vec3 vector, float scalar);
Vec3 vec3_mul(Vec3 left, Vec3 right);
float vec3_magnitude(Vec3 vector);
Quaternion vec3_degrees_to_quaternion(Vec3 degrees);

/*********************************************************
 *            MATRIX TRANSFORMATION FUNCTIONS            *
 *********************************************************/

Matrix matrix_create_scalar(Vec3 scalar);
Matrix matrix_create_rotation(Quaternion quaternion);
Matrix matrix_create_translation(Vec3 translation);
Matrix matrix_scale(Matrix matrix, Vec3 scalar);
Matrix matrix_rotate(Matrix matrix, Quaternion quaternion);
Matrix matrix_translate(Matrix matrix, Vec3 translation);

/*********************************************************
 *           MATRIX PROJECTION & VIEW FUNCTIONS          *
 *********************************************************/

Matrix matrix_ortho(float left, float right, float top, float bottom, float near, float far);
Matrix matrix_perspective(float aspect, float fov, float near, float far);
Matrix matrix_look_at(Vec3 eye, Vec3 target, Vec3 up);


#endif //SHLIB_SHLIB_INTERNAL_H
