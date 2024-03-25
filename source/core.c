#include "core.h"

int main(void) 
{
	render_engine_struct* re_struct = initialiseRenderEngine(1024, 768);
	if (re_struct == NULL) return 1;

	set_all_basic_3d_funtions(re_struct);
	set_basic_movement(re_struct);

	set_vec4(&re_struct->default_bg, 0.0f, 0.0f, 0.4f, 0.0f);

	MODEL_ID_TYPE model_id = addModel(re_struct, "./source/ext/objects/suzanne.obj", "./source/ext/textures/cubeuvmap.DDS");
	MODEL_INST_ID_TYPE inst_id = add_instance_of_model(re_struct, model_id);
	
	if (run(re_struct))
	{
		printf("Error while running\n");
	}

	free(re_struct);
	return 0;
}