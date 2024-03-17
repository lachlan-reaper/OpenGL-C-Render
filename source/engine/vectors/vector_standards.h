#ifndef VECTOR_STANDARDS_H
#define VECTOR_STANDARDS_H

#include <float.h>
#include <math.h>

#include <GL/gl.h>

#define get_vec2(VEC2_ARR, ROW) VEC2_ARR[ROW]
#define get_vec3(VEC3_ARR, ROW) VEC3_ARR[ROW]
#define get_vec4(VEC4_ARR, ROW) VEC4_ARR[ROW]
#define get_4x4(MAT_4X4_ARR, COL, ROW) MAT_4X4_ARR[4*COL + ROW]
#define deg_to_rad(DEG) (DEG * (M_PI / 180.0f))

typedef GLfloat VECTOR_FLT;
#define VECTOR_FLT_MIN FLT_MIN;
#define VECTOR_FLT_MAX FLT_MAX;

#endif