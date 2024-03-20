#ifndef VBO_INDEXER_H
#define VBO_INDEXER_H

#include "../vectors/vector2.h"
#include "../vectors/vector3.h"
#include "./dyn_array.h"

void indexVBO(
	const dyn_array* in_vertices,
	const dyn_array* in_uvs,
	const dyn_array* in_normals,

	dyn_array* out_indices,
	dyn_array* out_vertices,
	dyn_array* out_uvs,
	dyn_array* out_normals
);

#endif