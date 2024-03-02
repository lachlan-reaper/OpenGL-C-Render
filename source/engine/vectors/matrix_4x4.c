#include "matrix_4x4.h"

matrix_4x4* new_mat4x4()
{
	return calloc(1, sizeof(matrix_4x4));
}

void set_translate_mat4x4(matrix_4x4* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_4x4(matrix->arr, 0, 0) = 1;
	get_4x4(matrix->arr, 0, 1) = 0;
	get_4x4(matrix->arr, 0, 2) = 0;
	get_4x4(matrix->arr, 0, 3) = 0;

	get_4x4(matrix->arr, 1, 0) = 0;
	get_4x4(matrix->arr, 1, 1) = 1;
	get_4x4(matrix->arr, 1, 2) = 0;
	get_4x4(matrix->arr, 1, 3) = 0;

	get_4x4(matrix->arr, 2, 0) = 0;
	get_4x4(matrix->arr, 2, 1) = 0;
	get_4x4(matrix->arr, 2, 2) = 1;
	get_4x4(matrix->arr, 2, 3) = 0;

	get_4x4(matrix->arr, 3, 0) = delta_x;
	get_4x4(matrix->arr, 3, 1) = delta_y;
	get_4x4(matrix->arr, 3, 2) = delta_z;
	get_4x4(matrix->arr, 3, 3) = 1;
}
void set_scale_mat4x4(matrix_4x4* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_4x4(matrix->arr, 0, 0) = delta_x;
	get_4x4(matrix->arr, 0, 1) = 0;
	get_4x4(matrix->arr, 0, 2) = 0;
	get_4x4(matrix->arr, 0, 3) = 0;

	get_4x4(matrix->arr, 1, 0) = 0;
	get_4x4(matrix->arr, 1, 1) = delta_y;
	get_4x4(matrix->arr, 1, 2) = 0;
	get_4x4(matrix->arr, 1, 3) = 0;

	get_4x4(matrix->arr, 2, 0) = 0;
	get_4x4(matrix->arr, 2, 1) = 0;
	get_4x4(matrix->arr, 2, 2) = delta_z;
	get_4x4(matrix->arr, 2, 3) = 0;

	get_4x4(matrix->arr, 3, 0) = 0;
	get_4x4(matrix->arr, 3, 1) = 0;
	get_4x4(matrix->arr, 3, 2) = 0;
	get_4x4(matrix->arr, 3, 3) = 1;
}

/*
	WILL APPLY ROTATION IN ORDER M = B.X.Y.Z
*/
void set_rotate_mat4x4(matrix_4x4* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	VECTOR_FLT A = cos(delta_x);
	VECTOR_FLT B = sin(delta_x);
	VECTOR_FLT C = cos(delta_y);
	VECTOR_FLT D = sin(delta_y);
	VECTOR_FLT E = cos(delta_z);
	VECTOR_FLT F = sin(delta_z);

	VECTOR_FLT AD = A*D;
	VECTOR_FLT BD = B*D;

	get_4x4(matrix->arr, 0, 0) = C*E;
	get_4x4(matrix->arr, 0, 1) = BD*E + A*F;
	get_4x4(matrix->arr, 0, 2) = B*F - AD*E;
	get_4x4(matrix->arr, 0, 3) = 0;

	get_4x4(matrix->arr, 1, 0) = -C*F;
	get_4x4(matrix->arr, 1, 1) = A*E - BD*F;
	get_4x4(matrix->arr, 1, 2) = AD*F + B*E;
	get_4x4(matrix->arr, 1, 3) = 0;

	get_4x4(matrix->arr, 2, 0) = D;
	get_4x4(matrix->arr, 2, 1) = -B*C;
	get_4x4(matrix->arr, 2, 2) = A*C;
	get_4x4(matrix->arr, 2, 3) = 0;

	get_4x4(matrix->arr, 3, 0) = 0;
	get_4x4(matrix->arr, 3, 1) = 0;
	get_4x4(matrix->arr, 3, 2) = 0;
	get_4x4(matrix->arr, 3, 3) = 1;
}

void set_perspective_mat4x4(matrix_4x4* matrix, const VECTOR_FLT fov, const VECTOR_FLT aspect, const VECTOR_FLT _near, const VECTOR_FLT _far)
{
	VECTOR_FLT tan_fov = tan((VECTOR_FLT)fov/2.0f);
	if (tan_fov == 0)
	{
		get_4x4(matrix->arr, 0, 0) = 0;
		get_4x4(matrix->arr, 1, 1) = 0; // idk???
	}
	else
	{
		get_4x4(matrix->arr, 0, 0) = 1 / (aspect * tan_fov);
		get_4x4(matrix->arr, 1, 1) = 1 / tan_fov;
	}
	
	// get_4x4(matrix->arr, 0, 0) ^^
	get_4x4(matrix->arr, 0, 1) = 0;
	get_4x4(matrix->arr, 0, 2) = 0;
	get_4x4(matrix->arr, 0, 3) = 0;

	get_4x4(matrix->arr, 1, 0) = 0;
	// get_4x4(matrix->arr, 1, 1) ^^
	get_4x4(matrix->arr, 1, 2) = 0;
	get_4x4(matrix->arr, 1, 3) = 0;

	get_4x4(matrix->arr, 2, 0) = 0;
	get_4x4(matrix->arr, 2, 1) = 0;
	get_4x4(matrix->arr, 2, 2) = (VECTOR_FLT)(_far + _near) / (VECTOR_FLT)(_near - _far);
	get_4x4(matrix->arr, 2, 3) = -1;

	get_4x4(matrix->arr, 3, 0) = 0;
	get_4x4(matrix->arr, 3, 1) = 0;
	get_4x4(matrix->arr, 3, 2) = (VECTOR_FLT)(2*_far*_near) / (VECTOR_FLT)(_near - _far);
	get_4x4(matrix->arr, 3, 3) = 0;
}
void set_look_at_mat4x4(matrix_4x4* matrix, const vector3* location, const vector3* fixation, const vector3* rotation)
{
	vector3 x_axis;
	vector3 y_axis;
	vector3 z_axis;

	normalize_vec3(sub_vec3s(fixation, copy_to_vec3(location, &z_axis))); // Sets z-axis
	normalize_vec3(cross_vec3_by_vec3(&z_axis, copy_to_vec3(rotation, &x_axis))); // Sets x-axis
	cross_vec3_by_vec3(&x_axis, copy_to_vec3(&z_axis, &y_axis)); // Sets y-axis

	get_4x4(matrix->arr, 0, 0) = get_vec3(x_axis.arr, 0);
	get_4x4(matrix->arr, 0, 1) = get_vec3(y_axis.arr, 0);
	get_4x4(matrix->arr, 0, 2) = -get_vec3(z_axis.arr, 0);
	get_4x4(matrix->arr, 0, 3) = 0;

	get_4x4(matrix->arr, 1, 0) = get_vec3(x_axis.arr, 1);
	get_4x4(matrix->arr, 1, 1) = get_vec3(y_axis.arr, 1);
	get_4x4(matrix->arr, 1, 2) = -get_vec3(z_axis.arr, 1);
	get_4x4(matrix->arr, 1, 3) = 0;

	get_4x4(matrix->arr, 2, 0) = get_vec3(x_axis.arr, 2);
	get_4x4(matrix->arr, 2, 1) = get_vec3(y_axis.arr, 2);
	get_4x4(matrix->arr, 2, 2) = -get_vec3(z_axis.arr, 2);
	get_4x4(matrix->arr, 2, 3) = 0;

	get_4x4(matrix->arr, 3, 0) = -dot_vec3(&x_axis, location);
	get_4x4(matrix->arr, 3, 1) = -dot_vec3(&y_axis, location);
	get_4x4(matrix->arr, 3, 2) = dot_vec3(&z_axis, location);
	get_4x4(matrix->arr, 3, 3) = 1;
}
