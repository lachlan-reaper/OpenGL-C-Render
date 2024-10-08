#include "basic_3d_functions.h"

static render_engine_3d_info RE_3D_INFO; // Lasts the whole program and is on the heap not the stack

static inline void bindModel(const render_engine_struct* const re_struct, const Model* const model)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->Texture);
	glUniform1i(get_3d_info(re_struct).textureHandle, 0);

	glBindBuffer(GL_ARRAY_BUFFER, model->vertexbufferID);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, model->uvbufferID);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, model->normalbufferID);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indexbufferID);
}

static inline void bindInstancesAndDraw(const render_engine_struct* const re_struct, const Model* const model, const int size, const int instance_base)
{
	glUniformMatrix4fv(
		get_3d_info(re_struct).InstanceModelArrID, 
		size,
		GL_FALSE, 
		&get_4x4(dyn_get_4x4(model->instances_model_matrix.data, instance_base).arr, 0, 0)
	);

	glDrawElementsInstanced(
		GL_TRIANGLES, 
		model->indexes.current_size, 
		GL_UNSIGNED_INT, 
		(void*)0,
		model->instances_model_matrix.current_size
	);
}

MODEL_ID_TYPE addModel(render_engine_struct* const re_struct, const char* obj_path, const char* texture_path)
{
	Model* const model = add_slot_dyn_array(&(get_3d_info(re_struct).models));
	initialiseModel(model);
	loadObjectToModel(model, obj_path);
	loadTextureToModel(model, texture_path);

	{ // Generate VBO buffers
		// TODO: Maybe??? move all buffer related stuff to defaults (since it is dependent on shaders ig?)
		glGenBuffers(1, &model->vertexbufferID);
		glBindBuffer(GL_ARRAY_BUFFER, model->vertexbufferID);
		glBufferData(GL_ARRAY_BUFFER, model->indexed_vertices.current_size * sizeof(vector3), &dyn_get_vec3(model->indexed_vertices.data, 0), GL_STATIC_DRAW);

		glGenBuffers(1, &model->uvbufferID);
		glBindBuffer(GL_ARRAY_BUFFER, model->uvbufferID);
		glBufferData(GL_ARRAY_BUFFER, model->indexed_uvs.current_size * sizeof(vector2), &dyn_get_vec2(model->indexed_uvs.data, 0), GL_STATIC_DRAW);

		glGenBuffers(1, &model->normalbufferID);
		glBindBuffer(GL_ARRAY_BUFFER, model->normalbufferID);
		glBufferData(GL_ARRAY_BUFFER, model->indexed_normals.current_size * sizeof(vector3), &dyn_get_vec3(model->indexed_normals.data, 0), GL_STATIC_DRAW);

		glGenBuffers(1, &model->indexbufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indexbufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indexes.current_size * sizeof(unsigned int), &dyn_get_uint(model->indexes.data, 0), GL_STATIC_DRAW);
	}

	return get_3d_info(re_struct).models.current_size - 1;
}

/*
Returns instance id.
*/
MODEL_INST_ID_TYPE add_instance_of_model(render_engine_struct* const re_struct, const MODEL_ID_TYPE model_id, const vector3 coords, const vector3 scale, const vector3 rotation)
{
	return addModelInstance(dyn_get_void_ptr(&get_3d_info(re_struct).models, model_id), coords, scale, rotation);
}

int buffer_prime_function_3d(render_engine_struct* const re_struct)
{
	re_struct->programID = LoadShaders(re_struct->vertex_shader_path, re_struct->fragment_shader_path);

	glGenVertexArrays(1, &get_3d_info(re_struct).VertexArrayID);
	glBindVertexArray(get_3d_info(re_struct).VertexArrayID);
	glUseProgram(re_struct->programID);

	get_3d_info(re_struct).textureHandle = glGetUniformLocation(re_struct->programID, "myTextureSampler");

	get_3d_info(re_struct).VPMatrixID = glGetUniformLocation(re_struct->programID, "VP");
	get_3d_info(re_struct).ViewMatrixID = glGetUniformLocation(re_struct->programID, "V");
	get_3d_info(re_struct).InstanceModelArrID = glGetUniformLocation(re_struct->programID, "instance_M");

	get_3d_info(re_struct).LightID = glGetUniformLocation(re_struct->programID, "LightPosition_worldspace");
	set_vec3(&get_3d_info(re_struct).lightPos, 4, 4, 4);

	set_camera(&get_3d_info(re_struct).camera, deg_to_rad(24.0f), deg_to_rad(-24.0f), 45.0f);
	set_camera_position(&get_3d_info(re_struct).camera, -3, 3, -7);

	return 0;
}

int buffer_draw_function_3d(render_engine_struct* const re_struct)
{
	calc_camera_3d_vp(&get_3d_info(re_struct).camera, re_struct->window_width, re_struct->window_height);
	glUniformMatrix4fv(get_3d_info(re_struct).VPMatrixID, 1, GL_FALSE, &get_4x4(get_3d_info(re_struct).camera.VP.arr, 0, 0));
	glUniformMatrix4fv(get_3d_info(re_struct).ViewMatrixID, 1, GL_FALSE, &get_4x4(get_3d_info(re_struct).camera.view.arr, 0, 0));

	glUniform3f(get_3d_info(re_struct).LightID, get_vec3(get_3d_info(re_struct).lightPos.arr, 0), get_vec3(get_3d_info(re_struct).lightPos.arr, 1), get_vec3(get_3d_info(re_struct).lightPos.arr, 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (int i = 0; i < get_3d_info(re_struct).models.current_size; i++)
	{
		Model* const model = ((Model*)dyn_get_void_ptr(&get_3d_info(re_struct).models, i));
		bindModel(re_struct, model);

		for (int batch_base = 0; batch_base < model->instances_model_matrix.current_size; batch_base += MAX_INSTANCES_BATCH)
		{
			bindInstancesAndDraw(
				re_struct, 
				model, 
				min(model->instances_model_matrix.current_size - batch_base, MAX_INSTANCES_BATCH), 
				batch_base
			);
		}
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	return 0;
}

int buffer_clean_up_function_3d(render_engine_struct* const re_struct)
{
	for (int i = 0; i < get_3d_info(re_struct).models.current_size; i++) clean_model(dyn_get_void_ptr(&get_3d_info(re_struct).models, i));

	clean_dyn_array(&get_3d_info(re_struct).models);

	glDeleteVertexArrays(1, &get_3d_info(re_struct).VertexArrayID);

	return 0;
}

void set_all_basic_3d_funtions(render_engine_struct* const re_struct)
{
	re_struct->fragment_shader_path = "./source/engine/shaders/Standard3dShading.fragmentshader";
	re_struct->vertex_shader_path = "./source/engine/shaders/Standard3dShading.vertexshader";

	re_struct->buffer_data = (void*) &RE_3D_INFO;
	
	set_dyn_array(&get_3d_info(re_struct).models, DYN_ARRAY_NO_TYPE);
	override_item_size_dyn_array(&get_3d_info(re_struct).models, sizeof(Model));

	re_struct->buffer_prime_function = buffer_prime_function_3d;
	re_struct->buffer_draw_function = buffer_draw_function_3d;
	re_struct->buffer_clean_up_function = buffer_clean_up_function_3d;
}