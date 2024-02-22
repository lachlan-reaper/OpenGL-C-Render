#ifndef MATRIX_4SQ_H
#define MATRIX_4SQ_H

#include "vector_standards.h"
#include "vector3.h"
#include "vector4.h"

typedef struct matrix_4sq
{
	VECTOR_FLT arr[16];
} matrix_4sq;

matrix_4sq* new_mat4sq();

static inline void reset_mat4sq(matrix_4sq* matrix)
{
	for (int i = 0; i < 16; i++) matrix->arr[i] = 0;
}
static inline void set_identity_mat4sq(matrix_4sq* matrix)
{
	for (int i = 0; i < 16; i++) matrix->arr[i] = ((i / 4 == i % 4) ? 1 : 0);
}
void set_translate_mat4sq(matrix_4sq* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z);
void set_scale_mat4sq(matrix_4sq* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z);

/*
	delta_? must be in rads 
*/
void set_rotate_mat4sq(matrix_4sq* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z);

static inline matrix_4sq* transpose_mat4sq(const matrix_4sq* orig, matrix_4sq* transp)
{
	transp->arr[0] = orig->arr[0];
	transp->arr[1] = orig->arr[4];
	transp->arr[2] = orig->arr[8];
	transp->arr[3] = orig->arr[12];
	transp->arr[4] = orig->arr[1];
	transp->arr[5] = orig->arr[5];
	transp->arr[6] = orig->arr[9];
	transp->arr[7] = orig->arr[13];
	transp->arr[8] = orig->arr[2];
	transp->arr[9] = orig->arr[6];
	transp->arr[10] = orig->arr[10];
	transp->arr[11] = orig->arr[14];
	transp->arr[12] = orig->arr[3];
	transp->arr[13] = orig->arr[7];
	transp->arr[14] = orig->arr[11];
	transp->arr[15] = orig->arr[15];
	return (transp);
}

static matrix_4sq tmp_mat;

static inline void copy_to_tmp_buf_mat4sq(const matrix_4sq* base)
{
	tmp_mat.arr[0] = base->arr[0];
	tmp_mat.arr[1] = base->arr[1];
	tmp_mat.arr[2] = base->arr[2];
	tmp_mat.arr[3] = base->arr[3];
	tmp_mat.arr[4] = base->arr[4];
	tmp_mat.arr[5] = base->arr[5];
	tmp_mat.arr[6] = base->arr[6];
	tmp_mat.arr[7] = base->arr[7];
	tmp_mat.arr[8] = base->arr[8];
	tmp_mat.arr[9] = base->arr[9];
	tmp_mat.arr[10] = base->arr[10];
	tmp_mat.arr[11] = base->arr[11];
	tmp_mat.arr[12] = base->arr[12];
	tmp_mat.arr[13] = base->arr[13];
	tmp_mat.arr[14] = base->arr[14];
	tmp_mat.arr[15] = base->arr[15];
}

/*
	Result stored in second
*/
static inline matrix_4sq* cross_mat4sq_by_mat4sq(matrix_4sq* first, matrix_4sq* second)
{
	copy_to_tmp_buf_mat4sq(second);

	second->arr[0] = first->arr[0] * tmp_mat.arr[0] + first->arr[1] * tmp_mat.arr[4] + first->arr[2] * tmp_mat.arr[8] + first->arr[3] * tmp_mat.arr[12];
	second->arr[1] = first->arr[0] * tmp_mat.arr[1] + first->arr[1] * tmp_mat.arr[5] + first->arr[2] * tmp_mat.arr[9] + first->arr[3] * tmp_mat.arr[13];
	second->arr[2] = first->arr[0] * tmp_mat.arr[2] + first->arr[1] * tmp_mat.arr[6] + first->arr[2] * tmp_mat.arr[10] + first->arr[3] * tmp_mat.arr[14];
	second->arr[3] = first->arr[0] * tmp_mat.arr[3] + first->arr[1] * tmp_mat.arr[7] + first->arr[2] * tmp_mat.arr[11] + first->arr[3] * tmp_mat.arr[15];

	second->arr[4] = first->arr[4] * tmp_mat.arr[0] + first->arr[5] * tmp_mat.arr[4] + first->arr[6] * tmp_mat.arr[8] + first->arr[7] * tmp_mat.arr[12];
	second->arr[5] = first->arr[4] * tmp_mat.arr[1] + first->arr[5] * tmp_mat.arr[5] + first->arr[6] * tmp_mat.arr[9] + first->arr[7] * tmp_mat.arr[13];
	second->arr[6] = first->arr[4] * tmp_mat.arr[2] + first->arr[5] * tmp_mat.arr[6] + first->arr[6] * tmp_mat.arr[10] + first->arr[7] * tmp_mat.arr[14];
	second->arr[7] = first->arr[4] * tmp_mat.arr[3] + first->arr[5] * tmp_mat.arr[7] + first->arr[6] * tmp_mat.arr[11] + first->arr[7] * tmp_mat.arr[15];

	second->arr[8] = first->arr[8] * tmp_mat.arr[0] + first->arr[9] * tmp_mat.arr[4] + first->arr[10] * tmp_mat.arr[8] + first->arr[11] * tmp_mat.arr[12];
	second->arr[9] = first->arr[8] * tmp_mat.arr[1] + first->arr[9] * tmp_mat.arr[5] + first->arr[10] * tmp_mat.arr[9] + first->arr[11] * tmp_mat.arr[13];
	second->arr[10] = first->arr[8] * tmp_mat.arr[2] + first->arr[9] * tmp_mat.arr[6] + first->arr[10] * tmp_mat.arr[10] + first->arr[11] * tmp_mat.arr[14];
	second->arr[11] = first->arr[8] * tmp_mat.arr[3] + first->arr[9] * tmp_mat.arr[7] + first->arr[10] * tmp_mat.arr[11] + first->arr[11] * tmp_mat.arr[15];

	second->arr[12] = first->arr[12] * tmp_mat.arr[0] + first->arr[13] * tmp_mat.arr[4] + first->arr[14] * tmp_mat.arr[8] + first->arr[15] * tmp_mat.arr[12];
	second->arr[13] = first->arr[12] * tmp_mat.arr[1] + first->arr[13] * tmp_mat.arr[5] + first->arr[14] * tmp_mat.arr[9] + first->arr[15] * tmp_mat.arr[13];
	second->arr[14] = first->arr[12] * tmp_mat.arr[2] + first->arr[13] * tmp_mat.arr[6] + first->arr[14] * tmp_mat.arr[10] + first->arr[15] * tmp_mat.arr[14];
	second->arr[15] = first->arr[12] * tmp_mat.arr[3] + first->arr[13] * tmp_mat.arr[7] + first->arr[14] * tmp_mat.arr[11] + first->arr[15] * tmp_mat.arr[15];
	return second;
}
/*
	Result stored in vector
*/
static inline vector4* cross_mat4sq_by_vec4(matrix_4sq* matrix, vector4* vector)
{
	VECTOR_FLT x = vector->arr[0];
	VECTOR_FLT y = vector->arr[1];
	VECTOR_FLT z = vector->arr[2];
	VECTOR_FLT w = vector->arr[3];

	vector->arr[0] = x*matrix->arr[0] + y*matrix->arr[1] + z*matrix->arr[2] + w*matrix->arr[3];
	vector->arr[1] = x*matrix->arr[4] + y*matrix->arr[5] + z*matrix->arr[6] + w*matrix->arr[7];
	vector->arr[2] = x*matrix->arr[8] + y*matrix->arr[9] + z*matrix->arr[10] + w*matrix->arr[11];
	vector->arr[3] = x*matrix->arr[12] + y*matrix->arr[13] + z*matrix->arr[14] + w*matrix->arr[15];
	return vector;
}

static inline void translate_mat4sq(matrix_4sq* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	set_translate_mat4sq(&tmp_mat, delta_x, delta_y, delta_z);
	cross_mat4sq_by_mat4sq(&tmp_mat, matrix);
}
static inline void scale_mat4sq(matrix_4sq* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	set_scale_mat4sq(&tmp_mat, delta_x, delta_y, delta_z);
	cross_mat4sq_by_mat4sq(&tmp_mat, matrix);
}

/*
	delta_? must be in rads
*/
static inline void rotate_mat4sq(matrix_4sq* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	set_rotate_mat4sq(&tmp_mat, delta_x, delta_y, delta_z);
	cross_mat4sq_by_mat4sq(&tmp_mat, matrix);
}

void set_perspective_mat4sq(matrix_4sq* matrix, const VECTOR_FLT fov, const VECTOR_FLT aspect, const VECTOR_FLT near, const VECTOR_FLT far);
void set_look_at_mat4sq(matrix_4sq* matrix, const vector3* location, const vector3* fixation, const vector3* rotation);

static inline void transform_perspective_mat4sq(matrix_4sq* matrix, const VECTOR_FLT fov, const VECTOR_FLT aspect, const VECTOR_FLT _near, const VECTOR_FLT _far)
{
	matrix_4sq tmp_infunc_mat;
	set_perspective_mat4sq(&tmp_infunc_mat, fov, aspect, _near, _far);
	cross_mat4sq_by_mat4sq(&tmp_infunc_mat, matrix);
}
static inline void transform_look_at_mat4sq(matrix_4sq* matrix, const vector3* location, const vector3* fixation, const vector3* rotation)
{
	matrix_4sq tmp_infunc_mat;
	set_look_at_mat4sq(&tmp_infunc_mat, location, fixation, rotation);
	cross_mat4sq_by_mat4sq(&tmp_infunc_mat, matrix);
}

#endif