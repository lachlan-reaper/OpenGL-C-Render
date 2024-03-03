#include "vector2.h"

vector2* new_vec2(const VECTOR_FLT x, const VECTOR_FLT y)
{
	vector2* vector = calloc(1, sizeof(vector2));
	set_vec2(vector, x, y);
	return vector;
}
