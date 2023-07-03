//
// Created by Luis Tadeo Sanchez on 7/1/23.
//

#ifndef SHLIB_SHLIB_INTERNAL_H
#define SHLIB_SHLIB_INTERNAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    float x, y, z;
} Vec3;

typedef struct
{
    float x, y;
} Vec2;

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
} Graphics;

Matrix matrix_ortho(float left, float right, float top, float bottom, float near, float far);
Batch batch_create(int max_quads);

#endif //SHLIB_SHLIB_INTERNAL_H
