#ifndef BASIC_2D_FUNCTIONS_H
#define BASIC_2D_FUNCTIONS_H

#define MAX_INSTANCES_BATCH 200
#define get_2d_info(RE_STRUCT_PTR) (*((render_engine_2d_info*)(RE_STRUCT_PTR)->buffer_data))

#include "../render_engine.h"

#include "../util/shader_loader.h"

#include "../objects/camera.h"

typedef struct render_engine_2d_info {
	GLuint InstanceRotArrID;
	GLuint textureHandle;
	GLuint VertexArrayID;

	dyn_array shapes; // type: Shape
} render_engine_2d_info;

int buffer_prime_function_2d(render_engine_struct* const re_struct);
int buffer_draw_function_2d(render_engine_struct* const re_struct);
int buffer_clean_up_function_2d(render_engine_struct* const re_struct);

void set_all_basic_2d_funtions(render_engine_struct* const re_struct);

#endif