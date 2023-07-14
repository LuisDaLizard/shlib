//
// Created by Luis Tadeo Sanchez on 7/1/23.
//

#ifndef SHLIB_SHLIB_INTERNAL_H
#define SHLIB_SHLIB_INTERNAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

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
    GLFWwindow *handle;
    GLFWmonitor *monitor;
    int width;
    int height;
    bool fullscreen;
} Window;

typedef struct
{
    unsigned int vao, vbo, ebo;

    unsigned int num_triangles;
    unsigned int num_vertices;

    unsigned int max_quads;

    Vertex *vertices;
    unsigned int *indices;
} Batch;

typedef struct
{
    Matrix projection;
    Batch current_batch;
    Shader *base_shader;
} Graphics;

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

typedef struct
{
    Mesh **meshes;
    int num_meshes;
} Model;

struct aiNode;
struct aiMesh;
struct aiScene;

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

void window_resize_callback(GLFWwindow *handle, int width, int height);

/*********************************************************
 *                   GRAPHICS FUNCTIONS                  *
 *********************************************************/

void graphics_clear_screen(Vec4 color);
void graphics_begin_drawing(void);
void graphics_end_drawing(void);
void graphics_draw_demo(void);

/*********************************************************
 *                    SHADER FUNCTIONS                   *
 *********************************************************/

Shader *shader_load_from_memory(const char *vertex_src, const char *fragment_src);
Shader *shader_load_from_file(const char *vertex_path, const char *fragment_path);
void shader_unload(Shader *shader);
void shader_use(Shader *shader);
int shader_get_location(Shader *shader, const char *name);
void shader_set_uniform_vec3(Shader *shader, int location, Vec3 value);
void shader_set_uniform_matrix(Shader *shader, int location, Matrix value);

/*********************************************************
 *                     MESH FUNCTIONS                    *
 *********************************************************/

Mesh *mesh_create(Vertex *vertices, unsigned int *indices, int num_vertices, int num_indices);
void mesh_setup(Mesh *mesh);
void mesh_destroy(Mesh *mesh);

/*********************************************************
 *                    MODEL FUNCTIONS                    *
 *********************************************************/

Model *model_load_from_mesh(Mesh *mesh);
Model *model_load_from_file(const char *path);
void model_draw(Model *model);
void model_unload(Model *model);

void model_process_node(Model *model, struct aiNode *node, const struct aiScene *scene);
Mesh *model_process_mesh(Model *model, struct aiMesh *mesh, const struct aiScene *scene);

/*********************************************************
 *                  CORE MATH FUNCTIONS                  *
 *********************************************************/

Matrix matrix_identity();
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

/*********************************************************
 *                 FILE UTILITY FUNCTIONS                *
 *********************************************************/

char *file_read(const char *path);

#endif //SHLIB_SHLIB_INTERNAL_H
