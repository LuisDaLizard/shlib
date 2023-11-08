//
// Created by Luis Tadeo Sanchez on 7/2/23.
//

#include "shlib_internal.h"

#include <stdio.h>

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#define DEG2RAD (float)(M_PI / 180.0f)
#define RAD2DEG (float)(180.0f / M_PI)

Matrix matrix_identity(void)
{
    Matrix result = { 0 };
    result.m00 = 1;
    result.m11 = 1;
    result.m22 = 1;
    result.m33 = 1;
    return result;
}

Matrix matrix_mul(Matrix left, Matrix right)
{
    Matrix result;

    result.m00 = left.m00 * right.m00 + left.m01 * right.m10 + left.m02 * right.m20 + left.m03 * right.m30;
    result.m01 = left.m00 * right.m01 + left.m01 * right.m11 + left.m02 * right.m21 + left.m03 * right.m31;
    result.m02 = left.m00 * right.m02 + left.m01 * right.m12 + left.m02 * right.m22 + left.m03 * right.m32;
    result.m03 = left.m00 * right.m03 + left.m01 * right.m13 + left.m02 * right.m23 + left.m03 * right.m33;

    result.m10 = left.m10 * right.m00 + left.m11 * right.m10 + left.m12 * right.m20 + left.m13 * right.m30;
    result.m11 = left.m10 * right.m01 + left.m11 * right.m11 + left.m12 * right.m21 + left.m13 * right.m31;
    result.m12 = left.m10 * right.m02 + left.m11 * right.m12 + left.m12 * right.m22 + left.m13 * right.m32;
    result.m13 = left.m10 * right.m03 + left.m11 * right.m13 + left.m12 * right.m23 + left.m13 * right.m33;

    result.m20 = left.m20 * right.m00 + left.m21 * right.m10 + left.m22 * right.m20 + left.m23 * right.m30;
    result.m21 = left.m20 * right.m01 + left.m21 * right.m11 + left.m22 * right.m21 + left.m23 * right.m31;
    result.m22 = left.m20 * right.m02 + left.m21 * right.m12 + left.m22 * right.m22 + left.m23 * right.m32;
    result.m23 = left.m20 * right.m03 + left.m21 * right.m13 + left.m22 * right.m23 + left.m23 * right.m33;

    result.m30 = left.m30 * right.m00 + left.m31 * right.m10 + left.m32 * right.m20 + left.m33 * right.m30;
    result.m31 = left.m30 * right.m01 + left.m31 * right.m11 + left.m32 * right.m21 + left.m33 * right.m31;
    result.m32 = left.m30 * right.m02 + left.m31 * right.m12 + left.m32 * right.m22 + left.m33 * right.m32;
    result.m33 = left.m30 * right.m03 + left.m31 * right.m13 + left.m32 * right.m23 + left.m33 * right.m33;

    return result;
}

Matrix matrix_create_translation(Vec3 translation)
{
    Matrix result = matrix_identity();

    result.m03 = translation.x;
    result.m13 = translation.y;
    result.m23 = translation.z;

    return result;
}

Matrix matrix_create_scalar(Vec3 scalar)
{
    Matrix result = matrix_identity();

    result.m00 *= scalar.x;
    result.m11 *= scalar.y;
    result.m22 *= scalar.z;

    return result;
}

Matrix matrix_create_rotation(Vec3 axis, float degrees)
{
    float rads = degrees * DEG2RAD;
    float cosr = cosf(rads);
    float sinr = sinf(rads);

    Matrix result = matrix_identity();

    result.m00 = cosr + powf(axis.x, 2) * (1 - cosr);
    result.m01 = axis.x * axis.y * (1 - cosr) - axis.z * sinr;
    result.m02 = axis.x * axis.z * (1 - cosr) + axis.y * sinr;

    result.m10 = axis.y * axis.x * (1 - cosr) + axis.z * sinr;
    result.m11 = cosr + powf(axis.y, 2) * (1 - cosr);
    result.m12 = axis.y * axis.z * (1 - cosr) - axis.x * sinr;

    result.m20 = axis.z * axis.x * (1 - cosr) - axis.y * sinr;
    result.m21 = axis.z * axis.y * (1 - cosr) + axis.x * sinr;
    result.m22 = cosr + powf(axis.z, 2) * (1 - cosr);

    return result;
}

Matrix matrix_scale(Matrix matrix, Vec3 scalar)
{
    Matrix scale = matrix_create_scalar(scalar);

    matrix = matrix_mul(matrix, scale);

    return matrix;
}

Matrix matrix_rotate(Matrix matrix, Vec3 axis, float degrees)
{
    Matrix rotation = matrix_create_rotation(axis, degrees);

    matrix = matrix_mul(matrix, rotation);

    return matrix;
}

Matrix matrix_translate(Matrix matrix, Vec3 translation)
{
    Matrix translate = matrix_create_translation(translation);

    matrix = matrix_mul(matrix, translate);

    return matrix;
}

Vec3 matrix_mul_vec3(Matrix left, Vec3 right)
{
    Vec3 result;

    result.x = left.m00 * right.x + left.m01 * right.y + left.m02 * right.z + left.m03;
    result.y = left.m10 * right.x + left.m11 * right.y + left.m12 * right.z + left.m13;
    result.z = left.m20 * right.x + left.m21 * right.y + left.m22 * right.z + left.m23;

    return result;
}

void matrix_print(Matrix matrix)
{
    printf("|%f, %f, %f, %f|\n", matrix.m00, matrix.m01, matrix.m02, matrix.m03);
    printf("|%f, %f, %f, %f|\n", matrix.m10, matrix.m11, matrix.m12, matrix.m13);
    printf("|%f, %f, %f, %f|\n", matrix.m20, matrix.m21, matrix.m22, matrix.m23);
    printf("|%f, %f, %f, %f|\n", matrix.m30, matrix.m31, matrix.m32, matrix.m33);
}

void vec3_print(Vec3 vector)
{
    printf("(%f, %f, %f)\n", vector.x, vector.y, vector.z);
}

/*********************************************************
 *            VECTOR TRANSFORMATION FUNCTIONS            *
 *********************************************************/

Vec3 vec3_add(Vec3 left, Vec3 right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;

    return left;
}

Vec3 vec3_sub(Vec3 left, Vec3 right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;

    return left;
}

float vec3_dot(Vec3 left, Vec3 right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

Vec3 vec3_normalize(Vec3 vector)
{
    float magnitude = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

    vector.x /= magnitude;
    vector.y /= magnitude;
    vector.z /= magnitude;

    return vector;
}

Vec3 vec3_cross(Vec3 left, Vec3 right)
{
    Vec3 result = {};

    result.x = left.y * right.z - left.z * right.y;
    result.y = left.z * right.x - left.x * right.z;
    result.z = left.x * right.y - left.y * right.x;

    return result;
}

Vec3 vec3_negate(Vec3 vector)
{
    vector.x = -vector.x;
    vector.y = -vector.y;
    vector.z = -vector.z;

    return vector;
}

float vec3_magnitude(Vec3 vector)
{
    float mag = vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
    return sqrtf(mag);
}

Matrix matrix_ortho(float left, float right, float top, float bottom, float near, float far)
{
    Matrix result = matrix_identity();

    result.m00 = 2 / (right - left);
    result.m11 = 2 / (top - bottom);
    result.m22 = -2 / (far - near);

    result.m03 = -((right + left) / (right - left));
    result.m13 = -((top + bottom) / (top - bottom));
    result.m23 = -((far + near) / (far - near));

    return result;
}

Matrix matrix_perspective(float aspect, float fov, float near, float far)
{
    Matrix result = matrix_identity();

    float tan_fov = tanf((fov * DEG2RAD) / 2.0f);

    result.m00 = 1 / (aspect * tan_fov);
    result.m11 = 1 / tan_fov;
    result.m22 = -(far + near) / (far - near);
    result.m33 = 0;
    result.m23 = -(2 * far * near) / (far - near);
    result.m32 = -1;

    return result;
}

Matrix matrix_look_at(Vec3 eye, Vec3 target, Vec3 up)
{
    Matrix result = { 0 };

    Vec3 zaxis = vec3_normalize(vec3_sub(target, eye));
    Vec3 xaxis = vec3_normalize(vec3_cross(zaxis, up));
    Vec3 yaxis = vec3_cross(xaxis, zaxis);

    zaxis = vec3_negate(zaxis);

    result.m00 = xaxis.x;
    result.m01 = xaxis.y;
    result.m02 = xaxis.z;
    result.m03 = -vec3_dot(xaxis, eye);

    result.m10 = yaxis.x;
    result.m11 = yaxis.y;
    result.m12 = yaxis.z;
    result.m13 = -vec3_dot(yaxis, eye);

    result.m20 = zaxis.x;
    result.m21 = zaxis.y;
    result.m22 = zaxis.z;
    result.m23 = -vec3_dot(zaxis, eye);

    result.m33 = 1;

    return result;
}
