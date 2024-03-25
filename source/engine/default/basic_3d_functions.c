#include "basic_3d_functions.h"

int buffer_prime_function(render_engine_struct* re_struct)
{

	return 0;
}
int buffer_draw_function(render_engine_struct* re_struct)
{

	return 0;
}
int buffer_clean_up_function(render_engine_struct* re_struct)
{

	return 0;
}

void set_all_basic_3d_funtions(render_engine_struct* re_struct)
{
	re_struct->fragment_shader_path = "./source/engine/shaders/StandardShading.fragmentshader";
	re_struct->vertex_shader_path = "./source/engine/shaders/StandardShading.vertexshader";

	re_struct->buffer_prime_function = buffer_prime_function;
	re_struct->buffer_draw_function = buffer_draw_function;
	re_struct->buffer_clean_up_function = buffer_clean_up_function;
}