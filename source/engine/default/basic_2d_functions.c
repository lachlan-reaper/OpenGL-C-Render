#include "basic_2d_functions.h"

static render_engine_2d_info RE_2D_INFO; // Lasts the whole program and is on the heap not the stack

int buffer_prime_function_2d(render_engine_struct* const re_struct)
{
	re_struct->programID = LoadShaders(re_struct->vertex_shader_path, re_struct->fragment_shader_path);

	glGenVertexArrays(1, &get_2d_info(re_struct).VertexArrayID);
	glBindVertexArray(get_2d_info(re_struct).VertexArrayID);
	glUseProgram(re_struct->programID);

	get_2d_info(re_struct).textureHandle = glGetUniformLocation(re_struct->programID, "myTextureSampler");

	return 0;
}

int buffer_draw_function_2d(render_engine_struct* const re_struct)
{
	

	return 0;
}

int buffer_clean_up_function_2d(render_engine_struct* const re_struct)
{
	glDeleteVertexArrays(1, &get_2d_info(re_struct).VertexArrayID);

	return 0;
}

void set_all_basic_2d_funtions(render_engine_struct* const re_struct)
{
	re_struct->fragment_shader_path = "./source/engine/shaders/Standard2dShading.fragmentshader";
	re_struct->vertex_shader_path = "./source/engine/shaders/Standard2dShading.vertexshader";

	re_struct->buffer_data = (void*) &RE_2D_INFO;
	re_struct->buffer_prime_function = buffer_prime_function_2d;
	re_struct->buffer_draw_function = buffer_draw_function_2d;
	re_struct->buffer_clean_up_function = buffer_clean_up_function_2d;
}