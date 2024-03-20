#ifndef MODEL_H
#define MODEL_H

#include "../vectors/vector3.h"

#include "../util/dyn_array.h"
#include "../util/object_loader.h"
#include "../util/vbo_indexer.h"

typedef struct Model {
	dyn_array indexed_vertices;
	dyn_array indexed_normals;
	dyn_array indexed_uvs;
	dyn_array indexes;

	dyn_array instance_positions;
} Model;

void loadObjectToModel(Model* model, const char* path);

void clean_model(Model* model);

#endif