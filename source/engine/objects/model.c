#include "model.h"

static void recalc_instance_out_matrix(Model* const model, const MODEL_INST_ID_TYPE instance_id)
{
	struct model_matrix* const model_mat = (struct model_matrix*) dyn_get_void_ptr(&model->instances, instance_id);
	matrix_4x4* out_matrix = (matrix_4x4*) dyn_get_void_ptr(&model->instances_model_matrix, instance_id);

	const vector3 rotate_vec = model_mat->xyz_rotation;
	set_rotate_mat4x4(
		out_matrix, 
		get_vec3(rotate_vec.arr, 0),
		get_vec3(rotate_vec.arr, 1),
		get_vec3(rotate_vec.arr, 2)
	);
	
	const vector3 scale_vec = model_mat->xyz_scale;
	get_4x4(out_matrix->arr, 0, 0) *= get_vec3(scale_vec.arr, 0);
	get_4x4(out_matrix->arr, 0, 1) *= get_vec3(scale_vec.arr, 0);
	get_4x4(out_matrix->arr, 0, 2) *= get_vec3(scale_vec.arr, 0);

	get_4x4(out_matrix->arr, 1, 0) *= get_vec3(scale_vec.arr, 1);
	get_4x4(out_matrix->arr, 1, 1) *= get_vec3(scale_vec.arr, 1);
	get_4x4(out_matrix->arr, 1, 2) *= get_vec3(scale_vec.arr, 1);

	get_4x4(out_matrix->arr, 2, 0) *= get_vec3(scale_vec.arr, 2);
	get_4x4(out_matrix->arr, 2, 1) *= get_vec3(scale_vec.arr, 2);
	get_4x4(out_matrix->arr, 2, 2) *= get_vec3(scale_vec.arr, 2);

	const vector3 coords_vec = model_mat->xyz_coords;
	get_4x4(out_matrix->arr, 3, 0) = get_vec3(coords_vec.arr, 0);
	get_4x4(out_matrix->arr, 3, 1) = get_vec3(coords_vec.arr, 1);
	get_4x4(out_matrix->arr, 3, 2) = get_vec3(coords_vec.arr, 2);
}

Model* newModel()
{
	Model* model = (Model*) calloc(1, sizeof(Model));
	initialiseModel(model);
	return model;
}

void initialiseModel(Model* const model)
{
	set_dyn_array(&model->indexed_vertices, DYN_ARRAY_VECTOR_3_TYPE);
	set_dyn_array(&model->indexed_uvs, DYN_ARRAY_VECTOR_2_TYPE);
	set_dyn_array(&model->indexed_normals, DYN_ARRAY_VECTOR_3_TYPE);
	set_dyn_array(&model->indexes, DYN_ARRAY_UINT_TYPE);

	set_dyn_array(&model->instances, DYN_ARRAY_NO_TYPE);
	override_item_size_dyn_array(&model->instances, sizeof(struct model_matrix));
	set_dyn_array(&model->instances_model_matrix, DYN_ARRAY_MATRIX_4X4_TYPE);
}

void loadObjectToModel(Model* model, const char* path)
{
	dyn_array vertices;
	dyn_array uvs;
	dyn_array normals;

	// Read our .obj file
	set_dyn_array(&vertices, DYN_ARRAY_VECTOR_3_TYPE);
	set_dyn_array(&uvs, DYN_ARRAY_VECTOR_2_TYPE);
	set_dyn_array(&normals, DYN_ARRAY_VECTOR_3_TYPE);

	loadOBJ(path, &vertices, &uvs, &normals);

	{ // Reset dyn_arrays
		clean_dyn_array(&model->indexed_vertices);
		clean_dyn_array(&model->indexed_uvs);
		clean_dyn_array(&model->indexed_normals);
		clean_dyn_array(&model->indexes);
	}
	
	indexVBO(&vertices, &uvs, &normals, &model->indexes, &model->indexed_vertices, &model->indexed_uvs, &model->indexed_normals);
}

void loadTextureToModel(Model* model, const char* path)
{
	model->Texture = loadDDS(path);
}

/*
	Returns instance id.
*/
MODEL_INST_ID_TYPE addModelInstance(Model* const model, const vector3 coords, const vector3 scale, const vector3 rotation)
{
	add_slot_dyn_array(&model->instances);
	add_slot_dyn_array(&model->instances_model_matrix);
	((struct model_matrix*) dyn_get_last_void_ptr(&model->instances))->xyz_coords = coords;
	((struct model_matrix*) dyn_get_last_void_ptr(&model->instances))->xyz_scale = scale;
	((struct model_matrix*) dyn_get_last_void_ptr(&model->instances))->xyz_rotation = rotation;

	const MODEL_INST_ID_TYPE instance_id = model->instances.current_size - 1;

	recalc_instance_out_matrix(model, instance_id);

	return instance_id;
}

void clean_model(Model* model)
{
	clean_dyn_array(&model->indexed_vertices);
	clean_dyn_array(&model->indexed_uvs);
	clean_dyn_array(&model->indexed_normals);
	clean_dyn_array(&model->indexes);
	clean_dyn_array(&model->instances);
	clean_dyn_array(&model->instances_model_matrix);
	
	glDeleteBuffers(1, &model->vertexbufferID);
	glDeleteBuffers(1, &model->uvbufferID);
	glDeleteBuffers(1, &model->normalbufferID);
	glDeleteBuffers(1, &model->indexbufferID);

	glDeleteTextures(1, &model->Texture);
}
