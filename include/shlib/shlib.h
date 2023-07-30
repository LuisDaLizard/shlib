//
// Created by Luis Tadeo Sanchez on 7/1/23.
//

#ifndef SHLIB_SHLIB_H
#define SHLIB_SHLIB_H

#include <stdbool.h>

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
    Vec3 position;
    Vec3 normal;
    Vec2 tex_coord;
} Vertex;

typedef struct
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
} Matrix;

typedef void Mesh;
typedef void Shader;
typedef void Texture;
typedef void Framebuffer;

/*********************************************************
 *                    WINDOW FUNCTIONS                   *
 *********************************************************/

extern void window_init(int width, int height, const char *title);
extern void window_destroy(void);
extern bool window_should_close(void);
extern void window_poll_events(void);
extern void window_swap_buffers(void);
extern void window_toggle_fullscreen(void);
extern Vec2 window_get_size(void);
extern void window_disable_cursor(void);
extern void window_enable_cursor(void);

/*********************************************************
 *                     INPUT FUNCTIONS                   *
 *********************************************************/

extern Vec2 input_get_mouse_pos(void);

/*********************************************************
 *                   GRAPHICS FUNCTIONS                  *
 *********************************************************/

extern void graphics_clear_screen(Vec4 color);
extern void graphics_begin_drawing(void);
extern void graphics_end_drawing(void);
extern void graphics_enable_wireframe(void);
extern void graphics_disable_wireframe(void);
extern void graphics_enable_depth(void);
extern void graphics_disable_depth(void);
extern void graphics_enable_backface_culling(void);
extern void graphics_disable_backface_culling(void);
extern void graphics_enable_frontface_culling(void);
extern void graphics_disable_frontface_culling(void);
extern void graphics_enable_alpha_blending(void);
extern void graphics_disable_alpha_blending(void);

/*********************************************************
 *                    SHADER FUNCTIONS                   *
 *********************************************************/

extern Shader *shader_load(const char *vertex_src, const char *fragment_src);
extern void shader_unload(Shader *shader);
extern void shader_use(Shader *shader);
extern int shader_get_location(Shader *shader, const char *name);
extern void shader_set_uniform_vec3(Shader *shader, int location, Vec3 value);
extern void shader_set_uniform_matrix(Shader *shader, int location, Matrix value);

/*********************************************************
 *                   TEXTURE FUNCTIONS                   *
 *********************************************************/

extern Texture *texture_load(void *data, int width, int height, int channels);
extern void texture_unload(Texture *texture);
extern void texture_use(Texture *texture, int slot);

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

extern Mesh *mesh_create(Vertex *vertices, unsigned int *indices, int num_vertices, int num_indices);
extern void mesh_destroy(Mesh *mesh);
extern void mesh_draw(Mesh *mesh);

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

/*********************************************************
 *            MATRIX TRANSFORMATION FUNCTIONS            *
 *********************************************************/

extern Matrix matrix_create_scalar(Vec3 scalar);
extern Matrix matrix_create_rotation(Vec3 axis, float degrees);
extern Matrix matrix_create_translation(Vec3 translation);
extern Matrix matrix_scale(Matrix matrix, Vec3 scalar);
extern Matrix matrix_rotate(Matrix matrix, Vec3 axis, float degrees);
extern Matrix matrix_translate(Matrix matrix, Vec3 translation);

/*********************************************************
 *           MATRIX PROJECTION & VIEW FUNCTIONS          *
 *********************************************************/

extern Matrix matrix_ortho(float left, float right, float top, float bottom, float near, float far);
extern Matrix matrix_perspective(float aspect, float fov, float near, float far);
extern Matrix matrix_look_at(Vec3 eye, Vec3 target, Vec3 up);

#endif //SHLIB_SHLIB_H
