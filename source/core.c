#include "core.h"

static double current_time;
static double last_time;
int prime_function(render_engine_struct* re_struct)
{
	process_camera_movement(re_struct, 0.0f);
	current_time = glfwGetTime();
	last_time = current_time;
	return 0;
}
int process_function(render_engine_struct* re_struct)
{
	last_time = current_time;
	current_time = glfwGetTime();
	process_camera_movement(re_struct, (VECTOR_FLT)(current_time - last_time));
	return 0;
}

int main(void) 
{
	render_engine_struct* re_struct = initialiseRenderEngine(1024, 768);
	if (re_struct == NULL) return 1;

	// Set up standard info
	re_struct->fragment_shader_path = "./source/engine/shaders/StandardShading.fragmentshader";
	re_struct->vertex_shader_path = "./source/engine/shaders/StandardShading.vertexshader";
	re_struct->prime_function = prime_function;
	re_struct->process_function = process_function;

	if (run(re_struct))
	{
		printf("Error while running\n");
	}

	free(re_struct);
	return 0;
}