#ifndef BASIC_3D_FUNCTIONS_H
#define BASIC_3D_FUNCTIONS_H

#define MAX_INSTANCES_BATCH 200
#define get_3d_info(RE_STRUCT_PTR) (*((render_engine_3d_info*)(RE_STRUCT_PTR)->buffer_data))

#include "..\render_engine.h"
#include "..\objects\camera.h"

typedef struct render_engine_3d_info {
	vector3 lightPos;

	GLuint VPMatrixID;
	GLuint ViewMatrixID;
	GLuint InstanceModelArrID;
	GLuint textureHandle;
	GLuint VertexArrayID;

	GLuint LightID;
} render_engine_3d_info;

int buffer_prime_function(render_engine_struct* const re_struct);
int buffer_draw_function(render_engine_struct* const re_struct);
int buffer_clean_up_function(render_engine_struct* const re_struct);

void set_all_basic_3d_funtions(render_engine_struct* const re_struct);

#endif