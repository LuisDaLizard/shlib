//
// Created by Luis Tadeo Sanchez on 7/1/23.
//

#ifndef SHLIB_SHLIB_H
#define SHLIB_SHLIB_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>

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


typedef struct QuadVertex
{
    Vec3 position;
    Vec4 color;
    Vec2 tex_coord;
    float tex_id;
} QuadVertex;

typedef struct LineVertex
{
    Vec3 position;
    Vec4 color;
} LineVertex;

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

typedef struct Batch
{
    unsigned int max_elements;
    unsigned int num_quads;
    unsigned int num_lines;

    QuadVertex *quad_vertices;
    unsigned int *quad_indices;

    LineVertex *line_vertices;

    Texture **textures;
    unsigned int num_textures;

    unsigned int quad_vao, line_vao;
    unsigned int quad_vbo, line_vbo;
    unsigned int quad_ebo;
} Batch;

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


typedef enum MouseButtons
{
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
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
} Keys;


/*
 * Creates the window and initializes OpenGL
 */
extern void window_init(int width, int height, const char *title);

/*
 * Closes the window and destroys the OpenGL context
 */
extern void window_destroy(void);

/*
 * Returns whether the window should close
 */
extern bool window_should_close(void);

/*
 * Tells glfw to poll for input events
 */
extern void window_poll_events(void);

/*
 * Swaps window buffers
 */
extern void window_swap_buffers(void);

/*
 * Toggles fullscreen
 */
extern void window_toggle_fullscreen(void);

/*
 * Gets the current size of the window
 */
extern Vec2 window_get_size(void);

/*
 * Disables the mouse cursor
 */
extern void window_disable_cursor(void);

/*
 * Enables the mouse cursor
 */
extern void window_enable_cursor(void);

/*
 * Reads the given file as text
 */
extern char *utils_read_file(const char *path);

/*
 * Reads the given file in bytes
 */
extern unsigned char *utils_read_file_bytes(const char *path);

/*********************************************************
 *                     INPUT FUNCTIONS                   *
 *********************************************************/

extern Vec2 input_get_mouse_pos(void);
extern bool input_is_mouse_button_down(MouseButtons button);
extern bool input_is_key_down(Keys key);
extern double input_get_time();

/*********************************************************
 *                   GRAPHICS FUNCTIONS                  *
 *********************************************************/

extern void graphics_clear_screen(Vec4 color);
extern void graphics_draw_batch_quads(Batch *batch);
extern void graphics_draw_batch_lines(Batch *batch);
extern void graphics_draw_mesh(Mesh *mesh);

/*********************************************************
 *                    SHADER FUNCTIONS                   *
 *********************************************************/

extern Shader *shader_load_from_file(const char *vert_path, const char *frag_path);
extern Shader *shader_load(const char *vert_src, const char *frag_src);
extern void shader_use(Shader *shader);
extern void shader_unload(Shader *shader);
extern void shader_upload_int(Shader *shader, const char *name, int value);
extern void shader_upload_int_array(Shader *shader, const char *name, int n, int *value);
extern void shader_upload_float(Shader *shader, const char *name, float value);
extern void shader_upload_vec2(Shader *shader, const char *name, Vec2 value);
extern void shader_upload_vec3(Shader *shader, const char *name, Vec3 value);
extern void shader_upload_vec4(Shader *shader, const char *name, Vec4 value);
extern void shader_upload_matrix(Shader *shader, const char *name, Matrix value);

/*********************************************************
 *                   TEXTURE FUNCTIONS                   *
 *********************************************************/

extern Texture *texture_load_from_file(const char *path);
extern Texture *texture_load(void *data, int width, int height, int channels);
extern void texture_unload(Texture *texture);
extern void texture_use(Texture *texture, int slot);

/*********************************************************
 *                     BATCH FUNCTIONS                   *
 *********************************************************/

extern Batch *batch_create(unsigned int max_elements);
extern void batch_destroy(Batch *batch);
extern void batch_add_sprite(Batch *batch, Vec2 position, Vec2 size, Texture *texture);
extern void batch_add_sprite_uv(Batch *batch, Vec2 position, Vec2 size, Vec2 uv[4], Texture *texture);
extern void batch_add_text(Batch *batch, Vec2 position, Font *font, const char *text);
extern void batch_add_quad(Batch *batch, Vec2 position, Vec2 size, Vec4 color);
extern void batch_add_line(Batch *batch, Vec2 start, Vec2 end, Vec4 color, float width);

/*********************************************************
 *                     FONT FUNCTIONS                    *
 *********************************************************/

extern Font *font_load_from_file(const char *path, float font_size);
extern void font_unload(Font *font);

/*********************************************************
 *                 FRAMEBUFFER FUNCTIONS                 *
 *********************************************************/

extern Framebuffer *framebuffer_create_depth(int width, int height);
extern void framebuffer_destroy(Framebuffer *framebuffer);
extern void framebuffer_bind(Framebuffer *framebuffer);
extern void framebuffer_unbind(void);
extern Texture *framebuffer_get_texture(Framebuffer *framebuffer);

/*********************************************************
 *                     MESH FUNCTIONS                    *
 *********************************************************/

extern Mesh *mesh_create(Vertex3D *vertices, unsigned int *indices, int num_vertices, int num_indices);
extern void mesh_destroy(Mesh *mesh);

/*********************************************************
 *                  CORE MATH FUNCTIONS                  *
 *********************************************************/

extern Matrix matrix_identity(void);
extern Vec3 matrix_mul_vec3(Matrix left, Vec3 right);
extern Matrix matrix_mul(Matrix left, Matrix right);
extern void matrix_print(Matrix matrix);
extern void vec3_print(Vec3 vector);

/*********************************************************
 *            VECTOR TRANSFORMATION FUNCTIONS            *
 *********************************************************/

extern Vec3 vec3_add(Vec3 left, Vec3 right);
extern Vec3 vec3_sub(Vec3 left, Vec3 right);
extern float vec3_dot(Vec3 left, Vec3 right);
extern Vec3 vec3_normalize(Vec3 vector);
extern Vec3 vec3_cross(Vec3 left, Vec3 right);
extern Vec3 vec3_negate(Vec3 vector);
extern Vec3 vec3_scale(Vec3 vector, float scalar);
extern Vec3 vec3_mul(Vec3 left, Vec3 right);
extern float vec3_magnitude(Vec3 vector);
extern Quaternion vec3_degrees_to_quaternion(Vec3 degrees);

/*********************************************************
 *            MATRIX TRANSFORMATION FUNCTIONS            *
 *********************************************************/

extern Matrix matrix_create_scalar(Vec3 scalar);
extern Matrix matrix_create_rotation(Quaternion quaternion);
extern Matrix matrix_create_translation(Vec3 translation);
extern Matrix matrix_scale(Matrix matrix, Vec3 scalar);
extern Matrix matrix_rotate(Matrix matrix, Quaternion quaternion);
extern Matrix matrix_translate(Matrix matrix, Vec3 translation);

/*********************************************************
 *           MATRIX PROJECTION & VIEW FUNCTIONS          *
 *********************************************************/

extern Matrix matrix_ortho(float left, float right, float top, float bottom, float near, float far);
extern Matrix matrix_perspective(float aspect, float fov, float near, float far);
extern Matrix matrix_look_at(Vec3 eye, Vec3 target, Vec3 up);

#ifdef __cplusplus
}
#endif

#endif //SHLIB_SHLIB_H
