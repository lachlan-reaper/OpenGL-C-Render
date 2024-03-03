#ifndef VECTOR_STANDARDS_H
#define VECTOR_STANDARDS_H

#include <float.h>
#include <math.h>

#include <GL/gl.h>

#define get_vec2(VEC2, ROW) VEC2[ROW]
#define get_vec3(VEC3, ROW) VEC3[ROW]
#define get_vec4(VEC4, ROW) VEC4[ROW]
#define get_4x4(MAT_4X4, COL, ROW) MAT_4X4[4*COL + ROW]
#define deg_to_rad(DEG) (DEG * (M_PI / 180.0f))

typedef GLfloat VECTOR_FLT;
#define VECTOR_FLT_MIN FLT_MIN;
#define VECTOR_FLT_MAX FLT_MAX;

#endif