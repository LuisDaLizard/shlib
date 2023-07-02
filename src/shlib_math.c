//
// Created by Luis Tadeo Sanchez on 7/2/23.
//

#include <stdio.h>

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

Matrix matrix_identity()
{
    Matrix result = { 0 };
    result.m00 = 1;
    result.m11 = 1;
    result.m22 = 1;
    result.m33 = 1;
    return result;
}

Matrix matrix_translation(Vec3 translation)
{
    Matrix result = matrix_identity();

    result.m03 = translation.x;
    result.m13 = translation.y;
    result.m23 = translation.z;

    return result;
}

Vec3 matrix_mul_vec3(Matrix left, Vec3 right)
{
    Vec3 result;

    result.x = left.m00 * right.x + left.m01 * right.y + left.m02 * right.z + left.m03;
    result.y = left.m10 * right.x + left.m11 * right.y + left.m12 * right.z + left.m13;
    result.z = left.m20 * right.x + left.m21 * right.y + left.m22 * right.z + left.m23;

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

void matrix_print(Matrix matrix)
{
    printf("|%f, %f, %f, %f|\n", matrix.m00, matrix.m01, matrix.m02, matrix.m03);
    printf("|%f, %f, %f, %f|\n", matrix.m10, matrix.m11, matrix.m12, matrix.m13);
    printf("|%f, %f, %f, %f|\n", matrix.m20, matrix.m21, matrix.m22, matrix.m23);
    printf("|%f, %f, %f, %f|\n", matrix.m30, matrix.m31, matrix.m32, matrix.m33);
}
