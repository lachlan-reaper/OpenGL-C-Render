#ifndef VECTOR_4_H
#define VECTOR_4_H

#include <stdlib.h>
#include "vector_standards.h"

typedef struct vector4 
{
	VECTOR_FLT arr[4];
} vector4;

vector4* new_vec4(const VECTOR_FLT x, const VECTOR_FLT y, const VECTOR_FLT z, const VECTOR_FLT w);
inline void set_vec4(vector4* vector, const VECTOR_FLT x, const VECTOR_FLT y, const VECTOR_FLT z, const VECTOR_FLT w)
{
	vector.arr[0] = x;
	vector.arr[1] = y;
	vector.arr[2] = z;
	vector.arr[3] = w;
}

void translate_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z);
void scale_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z);

void rotate_vec4_x_axis_rad(vector4* vector, const VECTOR_FLT delta);
void rotate_vec4_y_axis_rad(vector4* vector, const VECTOR_FLT delta);
void rotate_vec4_z_axis_rad(vector4* vector, const VECTOR_FLT delta);
void rotate_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z);

inline void rotate_vec4_x_axis_deg(vector4* vector, const VECTOR_FLT delta)
{
	rotate_vec4_x_axis_rad(vector, deg_to_rad(delta));
}
inline void rotate_vec4_y_axis_deg(vector4* vector, const VECTOR_FLT delta)
{
	rotate_vec4_y_axis_rad(vector, deg_to_rad(delta));
}
inline void rotate_vec4_z_axis_deg(vector4* vector, const VECTOR_FLT delta)
{
	rotate_vec4_z_axis_rad(vector, deg_to_rad(delta));
}

inline void transform_vec4_rad(vector4* vector, 
	const VECTOR_FLT scale_delta_x, const VECTOR_FLT scale_delta_y, const VECTOR_FLT scale_delta_z,
	const VECTOR_FLT rotate_delta_x, const VECTOR_FLT rotate_delta_y, const VECTOR_FLT rotate_delta_z,
	const VECTOR_FLT translate_delta_x, const VECTOR_FLT translate_delta_y, const VECTOR_FLT translate_delta_z
) {
	scale_vec4(vector, scale_delta_x, scale_delta_y, scale_delta_z);
	rotate_vec4(vector, rotate_delta_x, rotate_delta_y, rotate_delta_z);
	translate_vec4(vector, translate_delta_x, translate_delta_y, translate_delta_z);
}

inline void transform_vec4_deg(vector4* vector, 
	const VECTOR_FLT scale_delta_x, const VECTOR_FLT scale_delta_y, const VECTOR_FLT scale_delta_z,
	const VECTOR_FLT rotate_delta_x, const VECTOR_FLT rotate_delta_y, const VECTOR_FLT rotate_delta_z,
	const VECTOR_FLT translate_delta_x, const VECTOR_FLT translate_delta_y, const VECTOR_FLT translate_delta_z
) {
	transform_vec4_rad(vector, 
		scale_delta_x, scale_delta_y, scale_delta_z,
		deg_to_rad(rotate_delta_x), deg_to_rad(rotate_delta_y), deg_to_rad(rotate_delta_z),
		translate_delta_x, translate_delta_y, translate_delta_z
	);
}


#endif