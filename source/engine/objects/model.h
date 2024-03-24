#ifndef MODEL_H
#define MODEL_H

#include "../vectors/vector3.h"
#include "../vectors/matrix_4x4.h"

#include "../util/dyn_array.h"
#include "../util/object_loader.h"
#include "../util/vbo_indexer.h"

#define MODEL_INST_ID_TYPE unsigned int

struct model_matrix {
	vector3 xyz_coords;
	vector3 xyz_scale;
	vector3 xyz_rotation;
	matrix_4x4 out_matrix;
};

typedef struct Model {
	dyn_array indexed_vertices;
	dyn_array indexed_normals;
	dyn_array indexed_uvs;
	dyn_array indexes;

	dyn_array instances; // type: struct model_matrix
} Model;

Model* newModel();
void initialiseModel(Model* const model);
void loadObjectToModel(Model* model, const char* path);

MODEL_INST_ID_TYPE addModelInstance(Model* const model, const vector3 coors, const vector3 scale, const vector3 rotation);

void clean_model(Model* model);

#endif