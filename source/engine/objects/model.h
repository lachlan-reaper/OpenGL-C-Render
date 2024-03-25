#ifndef MODEL_H
#define MODEL_H

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

#include "../vectors/vector3.h"
#include "../vectors/matrix_4x4.h"

#include "../util/dyn_array.h"
#include "../util/object_loader.h"
#include "../util/texture_loader.h"
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
	dyn_array instances_model_matrix; // type: matrix_4x4

	GLuint vertexbufferID;
	GLuint uvbufferID;
	GLuint normalbufferID;
	GLuint indexbufferID;

	GLuint Texture;
} Model;

Model* newModel();
void initialiseModel(Model* const model);
void loadObjectToModel(Model* model, const char* path);
void loadTextureToModel(Model* model, const char* path);

MODEL_INST_ID_TYPE addModelInstance(Model* const model, const vector3 coors, const vector3 scale, const vector3 rotation);

void clean_model(Model* model);

#endif