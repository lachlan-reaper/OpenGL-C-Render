#include "matrix_3x3.h"

matrix_3x3* new_mat3x3()
{
	return calloc(1, sizeof(matrix_3x3));
}

void set_translate_mat3x3(matrix_3x3* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y)
{
	get_3x3(matrix->arr, 0, 0) = 1;
	get_3x3(matrix->arr, 0, 1) = 0;
	get_3x3(matrix->arr, 0, 2) = 0;

	get_3x3(matrix->arr, 1, 0) = 0;
	get_3x3(matrix->arr, 1, 1) = 1;
	get_3x3(matrix->arr, 1, 2) = 0;

	get_3x3(matrix->arr, 2, 0) = delta_x;
	get_3x3(matrix->arr, 2, 1) = delta_y;
	get_3x3(matrix->arr, 2, 2) = 1;
}
void set_scale_mat3x3(matrix_3x3* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_3x3(matrix->arr, 0, 0) = delta_x;
	get_3x3(matrix->arr, 0, 1) = 0;
	get_3x3(matrix->arr, 0, 2) = 0;

	get_3x3(matrix->arr, 1, 0) = 0;
	get_3x3(matrix->arr, 1, 1) = delta_y;
	get_3x3(matrix->arr, 1, 2) = 0;

	get_3x3(matrix->arr, 2, 0) = 0;
	get_3x3(matrix->arr, 2, 1) = 0;
	get_3x3(matrix->arr, 2, 2) = delta_z;
}

/*
	TODO: NEED TO UPDATE
*/
void set_rotate_mat3x3(matrix_3x3* matrix, const VECTOR_FLT delta)
{
	VECTOR_FLT C = cos(delta);
	VECTOR_FLT S = sin(delta);
	
	get_3x3(matrix->arr, 0, 0) = C;
	get_3x3(matrix->arr, 0, 1) = S;
	get_3x3(matrix->arr, 0, 2) = 0;

	get_3x3(matrix->arr, 1, 0) = -S;
	get_3x3(matrix->arr, 1, 1) = C;
	get_3x3(matrix->arr, 1, 2) = 0;

	get_3x3(matrix->arr, 2, 0) = 0;
	get_3x3(matrix->arr, 2, 1) = 0;
	get_3x3(matrix->arr, 2, 2) = 1;
}
