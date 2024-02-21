#include "vector4.h"


vector4* new_vec4(const VECTOR_FLT x, const VECTOR_FLT y, const VECTOR_FLT z, const VECTOR_FLT w)
{
	vector4* vector = calloc(1, sizeof(vector4));
	set_vec4(vector, x, y, z, w);
	return vector;
}
void translate_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	vector.x += delta_x * vector.w;
	vector.y += delta_y * vector.w;
	vector.z += delta_z * vector.w;
}
void scale_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	vector.x *= delta_x;
	vector.y *= delta_y;
	vector.z *= delta_z;
}
void rotate_vec4_x_axis_rad(vector4* vector, const VECTOR_FLT delta)
{
	VECTOR_FLT tmp = vector.y*cos(delta) - vector.z*sin(delta);
	vector.z = vector.y*sin(delta) + vector.z*cos(delta);
	vector.y = tmp;
}
void rotate_vec4_y_axis_rad(vector4* vector, const VECTOR_FLT delta)
{
	VECTOR_FLT tmp = vector.z*cos(delta) - vector.x*sin(delta);
	vector.x = vector.z*sin(delta) + vector.x*cos(delta);
	vector.z = tmp;
}
void rotate_vec4_z_axis_rad(vector4* vector, const VECTOR_FLT delta)
{
	VECTOR_FLT tmp = vector.x*cos(delta) - vector.y*sin(delta);
	vector.y = vector.x*sin(delta) + vector.y*cos(delta);
	vector.x = tmp;
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

	VECTOR_FLT x = vector.x;
	VECTOR_FLT y = vector.y;
	VECTOR_FLT z = vector.z;

	vector.x = x*C*E - y*C*F + z*D;
	vector.y = x*(BD*E + A*F) + y*(-BD*F + A*E) - z*B*C;
	vector.z = x*(-AD*E + B*F) + y*(AD*F + B*E) + z*A*C;
}
