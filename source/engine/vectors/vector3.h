#ifndef VECTOR_3_H
#define VECTOR_3_H

#include <stdlib.h>
#include "vector_standards.h"

typedef struct vector3 
{
	VECTOR_FLT arr[3];
} vector3;

vector3* new_vec3(const VECTOR_FLT x, const VECTOR_FLT y, const VECTOR_FLT z);
static inline void set_vec3(vector3* vector, const VECTOR_FLT x, const VECTOR_FLT y, const VECTOR_FLT z)
{
	get_vec3(vector->arr, 0) = x;
	get_vec3(vector->arr, 1) = y;
	get_vec3(vector->arr, 2) = z;
}

static inline void translate_vec3(vector3* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_vec3(vector->arr, 0) += delta_x;
	get_vec3(vector->arr, 1) += delta_y;
	get_vec3(vector->arr, 2) += delta_z;
}
static inline void scale_vec3(vector3* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_vec3(vector->arr, 0) *= delta_x;
	get_vec3(vector->arr, 1) *= delta_y;
	get_vec3(vector->arr, 2) *= delta_z;
}
static inline void scale_vec3_xyz(vector3* vector, const VECTOR_FLT delta)
{
	get_vec3(vector->arr, 0) *= delta;
	get_vec3(vector->arr, 1) *= delta;
	get_vec3(vector->arr, 2) *= delta;
}

void rotate_vec3_x_axis_rad(vector3* vector, const VECTOR_FLT delta);
void rotate_vec3_y_axis_rad(vector3* vector, const VECTOR_FLT delta);
void rotate_vec3_z_axis_rad(vector3* vector, const VECTOR_FLT delta);

/*
	delta_? must be in rads
*/
void rotate_vec3(vector3* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z);

static inline void rotate_vec3_x_axis_deg(vector3* vector, const VECTOR_FLT delta)
{
	rotate_vec3_x_axis_rad(vector, deg_to_rad(delta));
}
static inline void rotate_vec3_y_axis_deg(vector3* vector, const VECTOR_FLT delta)
{
	rotate_vec3_y_axis_rad(vector, deg_to_rad(delta));
}
static inline void rotate_vec3_z_axis_deg(vector3* vector, const VECTOR_FLT delta)
{
	rotate_vec3_z_axis_rad(vector, deg_to_rad(delta));
}

static inline void transform_all_vec3_rad(vector3* vector, 
	const VECTOR_FLT scale_delta_x, const VECTOR_FLT scale_delta_y, const VECTOR_FLT scale_delta_z,
	const VECTOR_FLT rotate_delta_x, const VECTOR_FLT rotate_delta_y, const VECTOR_FLT rotate_delta_z,
	const VECTOR_FLT translate_delta_x, const VECTOR_FLT translate_delta_y, const VECTOR_FLT translate_delta_z
) {
	scale_vec3(vector, scale_delta_x, scale_delta_y, scale_delta_z);
	rotate_vec3(vector, rotate_delta_x, rotate_delta_y, rotate_delta_z);
	translate_vec3(vector, translate_delta_x, translate_delta_y, translate_delta_z);
}

static inline void transform_all_vec3_deg(vector3* vector, 
	const VECTOR_FLT scale_delta_x, const VECTOR_FLT scale_delta_y, const VECTOR_FLT scale_delta_z,
	const VECTOR_FLT rotate_delta_x, const VECTOR_FLT rotate_delta_y, const VECTOR_FLT rotate_delta_z,
	const VECTOR_FLT translate_delta_x, const VECTOR_FLT translate_delta_y, const VECTOR_FLT translate_delta_z
) {
	transform_all_vec3_rad(vector, 
		scale_delta_x, scale_delta_y, scale_delta_z,
		deg_to_rad(rotate_delta_x), deg_to_rad(rotate_delta_y), deg_to_rad(rotate_delta_z),
		translate_delta_x, translate_delta_y, translate_delta_z
	);
}

static inline vector3* copy_to_vec3(const vector3* base, vector3* copy)
{
	get_vec3(copy->arr, 0) = get_vec3(base->arr, 0);
	get_vec3(copy->arr, 1) = get_vec3(base->arr, 1);
	get_vec3(copy->arr, 2) = get_vec3(base->arr, 2);
	return copy;
}

static inline vector3* normalize_vec3(vector3* vector)
{
	VECTOR_FLT mag = sqrt(get_vec3(vector->arr, 0)*get_vec3(vector->arr, 0) + get_vec3(vector->arr, 1)*get_vec3(vector->arr, 1) + get_vec3(vector->arr, 2)*get_vec3(vector->arr, 2));
	if (mag != 0)
	{
		mag = 1 / mag;
		get_vec3(vector->arr, 0) *= mag;
		get_vec3(vector->arr, 1) *= mag;
		get_vec3(vector->arr, 2) *= mag;
	}
	return (vector);
}

static inline VECTOR_FLT dot_vec3(const vector3* first, const vector3* second)
{
	return (get_vec3(first->arr, 0)*get_vec3(second->arr, 0) + get_vec3(first->arr, 1)*get_vec3(second->arr, 1) + get_vec3(first->arr, 2)*get_vec3(second->arr, 2));
}

/*
	Result stored in second
*/
static inline vector3* add_vec3s(const vector3* first, vector3* second)
{
	get_vec3(second->arr, 0) += get_vec3(first->arr, 0);
	get_vec3(second->arr, 1) += get_vec3(first->arr, 1);
	get_vec3(second->arr, 2) += get_vec3(first->arr, 2);
	return second;
}

/*
	Result stored in second
*/
static inline vector3* sub_vec3s(const vector3* first, vector3* second)
{
	get_vec3(second->arr, 0) = get_vec3(first->arr, 0) - get_vec3(second->arr, 0);
	get_vec3(second->arr, 1) = get_vec3(first->arr, 1) - get_vec3(second->arr, 1);
	get_vec3(second->arr, 2) = get_vec3(first->arr, 2) - get_vec3(second->arr, 2);
	return second;
}

/*
	Result stored in second
*/
static inline vector3* cross_vec3_by_vec3(const vector3* first, vector3* second)
{
	VECTOR_FLT x = get_vec3(second->arr, 0);
	VECTOR_FLT y = get_vec3(second->arr, 1);
	VECTOR_FLT z = get_vec3(second->arr, 2);
	get_vec3(second->arr, 0) = get_vec3(first->arr, 1)*z - get_vec3(first->arr, 2)*y;
	get_vec3(second->arr, 1) = get_vec3(first->arr, 2)*x - get_vec3(first->arr, 0)*z;
	get_vec3(second->arr, 2) = get_vec3(first->arr, 0)*y - get_vec3(first->arr, 1)*x;
	return second;
}


#endif