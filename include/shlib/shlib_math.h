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

// ------------ Matrix Transformation Utility ------------- //

extern Matrix matrix_identity();

extern Matrix matrix_create_scalar(Vec3 scalar);
extern Matrix matrix_create_rotation(Vec3 axis, float degrees);
extern Matrix matrix_create_translation(Vec3 translation);

extern Matrix matrix_scale(Matrix matrix, Vec3 scalar);
extern Matrix matrix_rotate(Matrix matrix, Vec3 axis, float degrees);
extern Matrix matrix_translate(Matrix matrix, Vec3 translation);

extern Vec3 matrix_mul_vec3(Matrix left, Vec3 right);
extern Matrix matrix_mul(Matrix left, Matrix right);

//extern Vec3 vec3_normalize(Vec3 vector);

extern void matrix_print(Matrix matrix);
extern void vec3_print(Vec3 vector);

// ------------ Projection & View Matrix ----------- //

extern Matrix matrix_ortho(float left, float right, float top, float bottom, float near, float far);
//extern Matrix matrix_look_at(Vec3 eye, Vec3 target);

#endif //SHLIB_SHLIB_MATH_H
