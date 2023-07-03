//
// Created by Luis Tadeo Sanchez on 7/1/23.
//

#ifndef SHLIB_SHLIB_H
#define SHLIB_SHLIB_H

#include <stdbool.h>

typedef struct
{
    float x, y, z;
} Vec3;

typedef struct
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
} Matrix;

struct Shader;

/*********************************************************
 *                    WINDOW FUNCTIONS                   *
 *********************************************************/

extern void window_init(int width, int height, const char *title);
extern void window_destroy(void);
extern bool window_should_close(void);
extern void window_poll_events(void);
extern void window_swap_buffers(void);
extern void window_toggle_fullscreen(void);

/*********************************************************
 *                   GRAPHICS FUNCTIONS                  *
 *********************************************************/

extern void graphics_clear_screen(float color[4]);
extern void graphics_begin_drawing(void);
extern void graphics_end_drawing(void);
//extern void graphics_draw_quad(void);

/*********************************************************
 *                    SHADER FUNCTIONS                   *
 *********************************************************/

extern Shader shader_load_from_memory(const char *vertex_src, const char *fragment_src);
extern Shader shader_load_from_file(const char *vertex_path, const char *fragment_path);
extern void shader_unload(Shader shader);

/*********************************************************
 *                  CORE MATH FUNCTIONS                  *
 *********************************************************/

extern Matrix matrix_identity();
extern Vec3 matrix_mul_vec3(Matrix left, Vec3 right);
extern Matrix matrix_mul(Matrix left, Matrix right);
extern void matrix_print(Matrix matrix);
extern void vec3_print(Vec3 vector);

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
//extern Matrix matrix_look_at(Vec3 eye, Vec3 target);

/*********************************************************
 *                 FILE UTILITY FUNCTIONS                *
 *********************************************************/
 extern char *file_read(const char *path);

#endif //SHLIB_SHLIB_H
