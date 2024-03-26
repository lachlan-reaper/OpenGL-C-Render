#include "core.h"

int main(void) 
{
	render_engine_struct* re_struct = initialiseRenderEngine(1024, 768);
	if (re_struct == NULL) return 1;

	set_all_basic_3d_funtions(re_struct);
	set_basic_movement(re_struct);
	set_vec4(&re_struct->default_bg, 0.0f, 0.0f, 0.4f, 0.0f);

	MODEL_ID_TYPE model_id = addModel(re_struct, "./source/ext/objects/suzanne.obj", "./source/ext/textures/cubeuvmap.DDS");
	
	vector3 coords = {{0, 0, 0}};
	vector3 scale = {{1, 1, 1}};
	vector3 rotation = {{0, 0, 0}};

	const int space = 3;
	const int num_base = 10;
	const int num_sq = num_base * num_base;
	const int num_cu = num_base * num_sq;

	for (int i = 0; i < num_cu; i++)
	{
		get_vec3(coords.arr, 0) = space*(i % num_base);
		get_vec3(coords.arr, 1) = space*((i / num_base) % num_base);
		get_vec3(coords.arr, 2) = space*(i / num_sq);
		add_instance_of_model(re_struct, model_id, coords, scale, rotation);
	}
	
	if (run(re_struct))
	{
		printf("Error while running\n");
	}

	free(re_struct);
	return 0;
}