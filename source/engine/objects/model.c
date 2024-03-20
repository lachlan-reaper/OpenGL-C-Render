#include "model.h"

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

	set_dyn_array(&model->indexed_vertices, DYN_ARRAY_VECTOR_3_TYPE);
	set_dyn_array(&model->indexed_uvs, DYN_ARRAY_VECTOR_2_TYPE);
	set_dyn_array(&model->indexed_normals, DYN_ARRAY_VECTOR_3_TYPE);
	set_dyn_array(&model->indexes, DYN_ARRAY_UINT_TYPE);
	
	indexVBO(&vertices, &uvs, &normals, &model->indexes, &model->indexed_vertices, &model->indexed_uvs, &model->indexed_normals);
}

void clean_model(Model* model)
{
	clean_dyn_array(&model->indexed_vertices);
	clean_dyn_array(&model->indexed_uvs);
	clean_dyn_array(&model->indexed_normals);
	clean_dyn_array(&model->indexes);
}
