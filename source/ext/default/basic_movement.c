#include "basic_movement.h"

static double current_time;
static double last_time;
int prime_function(render_engine_struct* const re_struct)
{
	process_camera_movement(re_struct, 0.0f);
	current_time = glfwGetTime();
	last_time = current_time;
	return 0;
}
int process_function(render_engine_struct* const re_struct)
{
	last_time = current_time;
	current_time = glfwGetTime();
	
	process_camera_movement(re_struct, (VECTOR_FLT)(current_time - last_time));
	return 0;
}
int clean_up_function(render_engine_struct* const re_struct)
{
	return 0;
}

void set_basic_movement(render_engine_struct* const re_struct)
{
	re_struct->prime_function = prime_function;
	re_struct->process_function = process_function;
	re_struct->clean_up_function = clean_up_function;
}