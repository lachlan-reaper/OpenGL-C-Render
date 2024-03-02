#include "vector4.h"

vector4* new_vec4(const VECTOR_FLT x, const VECTOR_FLT y, const VECTOR_FLT z, const VECTOR_FLT w)
{
	vector4* vector = calloc(1, sizeof(vector4));
	set_vec4(vector, x, y, z, w);
	return vector;
}
void translate_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_vec4(vector->arr, 0) += delta_x * get_vec4(vector->arr, 3);
	get_vec4(vector->arr, 1) += delta_y * get_vec4(vector->arr, 3);
	get_vec4(vector->arr, 2) += delta_z * get_vec4(vector->arr, 3);
}
void scale_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_vec4(vector->arr, 0) *= delta_x;
	get_vec4(vector->arr, 1) *= delta_y;
	get_vec4(vector->arr, 2) *= delta_z;
}
void rotate_vec4_x_axis_rad(vector4* vector, const VECTOR_FLT delta)
{
	VECTOR_FLT tmp = get_vec4(vector->arr, 1)*cos(delta) - get_vec4(vector->arr, 2)*sin(delta);
	get_vec4(vector->arr, 2) = get_vec4(vector->arr, 1)*sin(delta) + get_vec4(vector->arr, 2)*cos(delta);
	get_vec4(vector->arr, 1) = tmp;
}
void rotate_vec4_y_axis_rad(vector4* vector, const VECTOR_FLT delta)
{
	VECTOR_FLT tmp = get_vec4(vector->arr, 2)*cos(delta) - get_vec4(vector->arr, 0)*sin(delta);
	get_vec4(vector->arr, 0) = get_vec4(vector->arr, 2)*sin(delta) + get_vec4(vector->arr, 0)*cos(delta);
	get_vec4(vector->arr, 2) = tmp;
}
void rotate_vec4_z_axis_rad(vector4* vector, const VECTOR_FLT delta)
{
	VECTOR_FLT tmp = get_vec4(vector->arr, 0)*cos(delta) - get_vec4(vector->arr, 1)*sin(delta);
	get_vec4(vector->arr, 1) = get_vec4(vector->arr, 0)*sin(delta) + get_vec4(vector->arr, 1)*cos(delta);
	get_vec4(vector->arr, 0) = tmp;
}

/*
	WILL APPLY ROTATION IN ORDER M = B.X.Y.Z
*/
void rotate_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	VECTOR_FLT A = cos(delta_x);
	VECTOR_FLT B = sin(delta_x);
	VECTOR_FLT C = cos(delta_y);
	VECTOR_FLT D = sin(delta_y);
	VECTOR_FLT E = cos(delta_z);
	VECTOR_FLT F = sin(delta_z);

	VECTOR_FLT AD = A*D;
	VECTOR_FLT BD = B*D;

	VECTOR_FLT x = get_vec4(vector->arr, 0);
	VECTOR_FLT y = get_vec4(vector->arr, 1);
	VECTOR_FLT z = get_vec4(vector->arr, 2);

	get_vec4(vector->arr, 0) = x*C*E - y*C*F + z*D;
	get_vec4(vector->arr, 1) = x*(BD*E + A*F) + y*(-BD*F + A*E) - z*B*C;
	get_vec4(vector->arr, 2) = x*(-AD*E + B*F) + y*(AD*F + B*E) + z*A*C;
}
