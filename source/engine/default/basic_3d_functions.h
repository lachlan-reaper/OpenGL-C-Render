#ifndef BASIC_3D_FUNCTIONS_H
#define BASIC_3D_FUNCTIONS_H

#define MODEL_ID_TYPE unsigned int

#define MAX_INSTANCES_BATCH 200
#define get_3d_info(RE_STRUCT_PTR) (*((render_engine_3d_info*)(RE_STRUCT_PTR)->buffer_data))

#include "../render_engine.h"

#include "../util/shader_loader.h"
#include "../util/texture_loader.h"

#include "../objects/camera.h"
#include "../objects/model.h"

typedef struct render_engine_3d_info {
	GLuint VPMatrixID;
	GLuint ViewMatrixID;
	GLuint InstanceModelArrID;
	GLuint textureHandle;
	GLuint VertexArrayID;
	GLuint LightID;

	Camera camera;
	vector3 lightPos;

	dyn_array models; // type: Model
} render_engine_3d_info;

MODEL_ID_TYPE addModel(render_engine_struct* const re_struct, const char* obj_path, const char* texture_path);
MODEL_INST_ID_TYPE add_instance_of_model(render_engine_struct* const re_struct, const MODEL_ID_TYPE model_id, const vector3 coords, const vector3 scale, const vector3 rotation);

int buffer_prime_function_3d(render_engine_struct* const re_struct);
int buffer_draw_function_3d(render_engine_struct* const re_struct);
int buffer_clean_up_function_3d(render_engine_struct* const re_struct);

void set_all_basic_3d_funtions(render_engine_struct* const re_struct);

#endif