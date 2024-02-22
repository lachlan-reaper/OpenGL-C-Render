#include "matrix_4x4.h"

matrix_4x4* new_mat4x4()
{
	return calloc(1, sizeof(matrix_4x4));
}

void set_translate_mat4x4(matrix_4x4* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	matrix->arr[0] = 1;
	matrix->arr[1] = 0;
	matrix->arr[2] = 0;
	matrix->arr[3] = delta_x;

	matrix->arr[4] = 0;
	matrix->arr[5] = 1;
	matrix->arr[6] = 0;
	matrix->arr[7] = delta_y;

	matrix->arr[8] = 0;
	matrix->arr[9] = 0;
	matrix->arr[10] = 1;
	matrix->arr[11] = delta_z;

	matrix->arr[12] = 0;
	matrix->arr[13] = 0;
	matrix->arr[14] = 0;
	matrix->arr[15] = 1;
}
void set_scale_mat4x4(matrix_4x4* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	matrix->arr[0] = delta_x;
	matrix->arr[1] = 0;
	matrix->arr[2] = 0;
	matrix->arr[3] = 0;

	matrix->arr[4] = 0;
	matrix->arr[5] = delta_y;
	matrix->arr[6] = 0;
	matrix->arr[7] = 0;

	matrix->arr[8] = 0;
	matrix->arr[9] = 0;
	matrix->arr[10] = delta_z;
	matrix->arr[11] = 0;

	matrix->arr[12] = 0;
	matrix->arr[13] = 0;
	matrix->arr[14] = 0;
	matrix->arr[15] = 1;
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

	matrix->arr[0] = C*E;
	matrix->arr[1] = -C*F;
	matrix->arr[2] = D;
	matrix->arr[3] = 0;

	matrix->arr[4] = BD*E + A*F;
	matrix->arr[5] = A*E - BD*F;
	matrix->arr[6] = -B*C;
	matrix->arr[7] = 0;

	matrix->arr[8] = B*F - AD*E;
	matrix->arr[9] = AD*F + B*E;
	matrix->arr[10] = A*C;
	matrix->arr[11] = 0;

	matrix->arr[12] = 0;
	matrix->arr[13] = 0;
	matrix->arr[14] = 0;
	matrix->arr[15] = 1;
}

void set_perspective_mat4x4(matrix_4x4* matrix, const VECTOR_FLT fov, const VECTOR_FLT aspect, const VECTOR_FLT _near, const VECTOR_FLT _far)
{
	VECTOR_FLT tan_fov = tan((VECTOR_FLT)fov/2.0f);
	if (tan_fov == 0)
	{
		matrix->arr[0] = 0;
		matrix->arr[5] = 0; // idk???
	}
	else
	{
		matrix->arr[0] = 1 / (aspect * tan_fov);
		matrix->arr[5] = 1 / tan_fov;
	}
	
	// matrix->arr[0] ^^
	matrix->arr[1] = 0;
	matrix->arr[2] = 0;
	matrix->arr[3] = 0;

	matrix->arr[4] = 0;
	// matrix->arr[5] ^^
	matrix->arr[6] = 0;
	matrix->arr[7] = 0;

	matrix->arr[8] = 0;
	matrix->arr[9] = 0;
	matrix->arr[10] = (VECTOR_FLT)(_far + _near) / (VECTOR_FLT)(_near - _far);
	matrix->arr[11] = (VECTOR_FLT)(2*_far*_near) / (VECTOR_FLT)(_near - _far);

	matrix->arr[12] = 0;
	matrix->arr[13] = 0;
	matrix->arr[14] = -1;
	matrix->arr[15] = 0;
}
void set_look_at_mat4x4(matrix_4x4* matrix, const vector3* location, const vector3* fixation, const vector3* rotation)
{
	vector3 x_axis;
	vector3 y_axis;
	vector3 z_axis;

	normalize_vec3(sub_vec3s(fixation, copy_to_vec3(location, &z_axis))); // Sets z-axis
	normalize_vec3(cross_vec3_by_vec3(&z_axis, copy_to_vec3(rotation, &x_axis))); // Sets x-axis
	cross_vec3_by_vec3(&x_axis, copy_to_vec3(&z_axis, &y_axis)); // Sets y-axis

	matrix->arr[0] = x_axis.arr[0];
	matrix->arr[1] = x_axis.arr[1];
	matrix->arr[2] = x_axis.arr[2];
	matrix->arr[3] = -dot_vec3(&x_axis, location);

	matrix->arr[4] = y_axis.arr[0];
	matrix->arr[5] = y_axis.arr[1];
	matrix->arr[6] = y_axis.arr[2];
	matrix->arr[7] = -dot_vec3(&y_axis, location);

	matrix->arr[8] = -z_axis.arr[0];
	matrix->arr[9] = -z_axis.arr[1];
	matrix->arr[10] = -z_axis.arr[2];
	matrix->arr[11] = dot_vec3(&z_axis, location);

	matrix->arr[12] = 0;
	matrix->arr[13] = 0;
	matrix->arr[14] = 0;
	matrix->arr[15] = 1;
}
