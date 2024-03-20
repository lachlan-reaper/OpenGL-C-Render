#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include <stdio.h>

#include "../vectors/vector_standards.h"
#include "../vectors/vector2.h"
#include "../vectors/vector3.h"
#include "./dyn_array.h"

int loadOBJ(
	const char * path, 
	dyn_array* out_vertices, 
	dyn_array* out_uvs, 
	dyn_array* out_normals
);

#endif