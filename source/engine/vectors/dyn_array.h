#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include "./vector_standards.h"
#include "./vector2.h"
#include "./vector3.h"
#include "./vector4.h"
#include "./matrix_4x4.h"

enum dyn_array_type {
	DYN_ARRAY_INT_TYPE,
	DYN_ARRAY_UINT_TYPE,
	DYN_ARRAY_VECTOR_2_TYPE,
	DYN_ARRAY_VECTOR_3_TYPE,
	DYN_ARRAY_VECTOR_4_TYPE,
	DYN_ARRAY_MATRIX_4X4_TYPE
};

typedef struct dyn_array {
	enum dyn_array_type type;
	unsigned int current_size;
	unsigned int max_size;
	void* data;
} dyn_array;

#define dyn_get_int(DYN_ARRAY_DATA, INDEX) ((int*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_uint(DYN_ARRAY_DATA, INDEX) ((unsigned int*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_vec2(DYN_ARRAY_DATA, INDEX) ((vector2*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_vec3(DYN_ARRAY_DATA, INDEX) ((vector3*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_vec4(DYN_ARRAY_DATA, INDEX) ((vector4*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_4x4(DYN_ARRAY_DATA, INDEX) ((matrix_4x4*)DYN_ARRAY_DATA)[INDEX]

#define dyn_get_last_int(DYN_ARRAY_STRUCT_PTR) dyn_get_int((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_uint(DYN_ARRAY_STRUCT_PTR) dyn_get_uint((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_vec2(DYN_ARRAY_STRUCT_PTR) dyn_get_vec2((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_vec3(DYN_ARRAY_STRUCT_PTR) dyn_get_vec3((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_vec4(DYN_ARRAY_STRUCT_PTR) dyn_get_vec4((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_4x4(DYN_ARRAY_STRUCT_PTR) dyn_get_4x4((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)

static inline void set_dyn_array(dyn_array* dyn_struct, const enum dyn_array_type type)
{
	dyn_struct->type = type;
	dyn_struct->current_size = 0;
	dyn_struct->max_size = 0;
	dyn_struct->data = NULL;
}

static inline void* get_dyn_array(const dyn_array* dyn_struct, unsigned int index)
{
	if (dyn_struct == NULL) return NULL;
	if (index >= dyn_struct->current_size) return NULL;

	switch (dyn_struct->type)
	{
		case DYN_ARRAY_INT_TYPE:
			return &dyn_get_int(dyn_struct->data, index);
		case DYN_ARRAY_UINT_TYPE:
			return &dyn_get_uint(dyn_struct->data, index);
		case DYN_ARRAY_VECTOR_2_TYPE:
			return &dyn_get_vec2(dyn_struct->data, index);
		case DYN_ARRAY_VECTOR_3_TYPE:
			return &dyn_get_vec3(dyn_struct->data, index);
		case DYN_ARRAY_VECTOR_4_TYPE:
			return &dyn_get_vec4(dyn_struct->data, index);
		case DYN_ARRAY_MATRIX_4X4_TYPE:
			return &dyn_get_4x4(dyn_struct->data, index);
	}
}

static inline void* get_last_dyn_array(const dyn_array* dyn_struct)
{
	if (dyn_struct == NULL) return NULL;
	if (dyn_struct->current_size == 0) return NULL;

	switch (dyn_struct->type)
	{
		case DYN_ARRAY_INT_TYPE:
			return &dyn_get_last_int(dyn_struct);
		case DYN_ARRAY_UINT_TYPE:
			return &dyn_get_last_uint(dyn_struct);
		case DYN_ARRAY_VECTOR_2_TYPE:
			return &dyn_get_last_vec2(dyn_struct);
		case DYN_ARRAY_VECTOR_3_TYPE:
			return &dyn_get_last_vec3(dyn_struct);
		case DYN_ARRAY_VECTOR_4_TYPE:
			return &dyn_get_last_vec4(dyn_struct);
		case DYN_ARRAY_MATRIX_4X4_TYPE:
			return &dyn_get_last_4x4(dyn_struct);
	}
	
	return NULL;
}

static inline void* add_slot_dyn_array(dyn_array* dyn_struct)
{
	if (dyn_struct->current_size == dyn_struct->max_size)
	{
		dyn_struct->max_size = 2 * dyn_struct->max_size + 1;
		size_t size;
		switch (dyn_struct->type)
		{
			case DYN_ARRAY_INT_TYPE:
				size = sizeof(int);
				break;
			case DYN_ARRAY_UINT_TYPE:
				size = sizeof(unsigned int);
				break;
			case DYN_ARRAY_VECTOR_2_TYPE:
				size = sizeof(vector2);
				break;
			case DYN_ARRAY_VECTOR_3_TYPE:
				size = sizeof(vector3);
				break;
			case DYN_ARRAY_VECTOR_4_TYPE:
				size = sizeof(vector4);
				break;
			case DYN_ARRAY_MATRIX_4X4_TYPE:
				size = sizeof(matrix_4x4);
				break;
			default:
				size = 1;
				break;
		}
		dyn_struct->data = realloc(dyn_struct->data, dyn_struct->max_size * size);
	}

	dyn_struct->current_size++;
	return get_last_dyn_array(dyn_struct);
}

#endif