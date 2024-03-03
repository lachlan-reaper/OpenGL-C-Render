#include "core.h"

static double current_time;
static double last_time;
int prime_function(render_engine_struct* re_struct)
{
	current_time = glfwGetTime();
	last_time = current_time;
	process_camera_movement(re_struct->window, &re_struct->camera, 0.0f);
	return 0;
}
int process_function(render_engine_struct* re_struct)
{
	last_time = current_time;
	current_time = glfwGetTime();
	process_camera_movement(re_struct->window, &re_struct->camera, (VECTOR_FLT)(current_time - last_time));
	return 0;
}

int main(void) 
{
	render_engine_struct* re_struct = initialiseRenderEngine();
	if (re_struct == NULL) return 1;

	re_struct->prime_function = prime_function;
	re_struct->process_function = process_function;

	run(re_struct);

	free(re_struct);
	return 0;
}