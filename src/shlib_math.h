//
// Created by Luis Tadeo Sanchez on 7/2/23.
//

#ifndef SHLIB_SHLIB_MATH_H
#define SHLIB_SHLIB_MATH_H

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

Matrix matrix_identity();
Matrix matrix_translation(Vec3 translation);
Vec3 matrix_mul_vec3(Matrix left, Vec3 right);
Matrix matrix_mul(Matrix left, Matrix right);
void matrix_print(Matrix matrix);

#endif //SHLIB_SHLIB_MATH_H