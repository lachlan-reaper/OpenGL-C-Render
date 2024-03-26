#include "matrix_2x2.h"

matrix_2x2* new_mat2x2()
{
	return calloc(1, sizeof(matrix_2x2));
}

void set_translate_mat2x2(matrix_2x2* matrix, const VECTOR_FLT delta)
{
	get_2x2(matrix->arr, 0, 0) = 1;
	get_2x2(matrix->arr, 0, 1) = 0;

	get_2x2(matrix->arr, 1, 0) = delta;
	get_2x2(matrix->arr, 1, 1) = 1;
}
void set_scale_mat2x2(matrix_2x2* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y)
{
	get_2x2(matrix->arr, 0, 0) = delta_x;
	get_2x2(matrix->arr, 0, 1) = 0;

	get_2x2(matrix->arr, 1, 0) = 0;
	get_2x2(matrix->arr, 1, 1) = delta_y;
}