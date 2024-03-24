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
	// Cleanup VBO
	glDeleteBuffers(1, &re_struct->ids.vertexbuffer);
	glDeleteBuffers(1, &re_struct->ids.uvbuffer);
	glDeleteBuffers(1, &re_struct->ids.normalbuffer);
	glDeleteBuffers(1, &re_struct->ids.indexbuffer);
	glDeleteProgram(re_struct->ids.programID);
	glDeleteTextures(1, &re_struct->ids.Texture);

	for (int i = re_struct->models.current_size - 1; i>=0; i--) clean_model(dyn_get_void_ptr(&re_struct->models, i));

	return 0;
}

void set_all_basic_3d_funtions(render_engine_struct* re_struct)
{
	// Set up standard info
	re_struct->fragment_shader_path = "./source/engine/shaders/StandardShading.fragmentshader";
	re_struct->vertex_shader_path = "./source/engine/shaders/StandardShading.vertexshader";

	re_struct->buffer_prime_function = buffer_prime_function;
	re_struct->buffer_draw_function = buffer_draw_function;
	re_struct->buffer_clean_up_function = buffer_clean_up_function;
}