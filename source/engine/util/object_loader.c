#include "object_loader.h"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

int loadOBJ(
	const char * path, 
	dyn_array* out_vertices, 
	dyn_array* out_uvs, 
	dyn_array* out_normals
) {
	printf("Loading OBJ file %s...\n", path);

	dyn_array vertexIndices;
	dyn_array uvIndices;
	dyn_array normalIndices;
	set_dyn_array(&vertexIndices, DYN_ARRAY_UINT_TYPE);
	set_dyn_array(&uvIndices, DYN_ARRAY_UINT_TYPE);
	set_dyn_array(&normalIndices, DYN_ARRAY_UINT_TYPE);

	dyn_array temp_vertices; 
	dyn_array temp_uvs;
	dyn_array temp_normals;
	set_dyn_array(&temp_vertices, DYN_ARRAY_VECTOR_3_TYPE);
	set_dyn_array(&temp_uvs, DYN_ARRAY_VECTOR_2_TYPE);
	set_dyn_array(&temp_normals, DYN_ARRAY_VECTOR_3_TYPE);

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file! Filepath: '%s'\n", path);
		getchar();
		return 0;
	}

	char lineHeader[256];
	while( 1 ){

		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			vector3 vertex;
			fscanf(file, "%f %f %f\n", &get_vec3(vertex.arr, 0), &get_vec3(vertex.arr, 1), &get_vec3(vertex.arr, 2));
			copy_to_vec3(&vertex, add_slot_dyn_array(&temp_vertices));
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			vector2 uv;
			fscanf(file, "%f %f\n", &get_vec2(uv.arr, 0), &get_vec2(uv.arr, 1));
			get_vec2(uv.arr, 1) = -get_vec2(uv.arr, 1); // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			copy_to_vec2(&uv, add_slot_dyn_array(&temp_uvs));
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			vector3 normal;
			fscanf(file, "%f %f %f\n", &get_vec3(normal.arr, 0), &get_vec3(normal.arr, 1), &get_vec3(normal.arr, 2));
			copy_to_vec3(&normal, add_slot_dyn_array(&temp_normals));
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser\n");
				fclose(file);
				return 0;
			}

			*((unsigned int*)add_slot_dyn_array(&vertexIndices)) = vertexIndex[0];
			*((unsigned int*)add_slot_dyn_array(&vertexIndices)) = vertexIndex[1];
			*((unsigned int*)add_slot_dyn_array(&vertexIndices)) = vertexIndex[2];
			*((unsigned int*)add_slot_dyn_array(&uvIndices)) = uvIndex[0];
			*((unsigned int*)add_slot_dyn_array(&uvIndices)) = uvIndex[1];
			*((unsigned int*)add_slot_dyn_array(&uvIndices)) = uvIndex[2];
			*((unsigned int*)add_slot_dyn_array(&normalIndices)) = normalIndex[0];
			*((unsigned int*)add_slot_dyn_array(&normalIndices)) = normalIndex[1];
			*((unsigned int*)add_slot_dyn_array(&normalIndices)) = normalIndex[2];
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}
	fclose(file);

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.current_size; i++ )
	{
		// Put the attributes in buffers
		copy_to_vec3(
			&dyn_get_vec3(temp_vertices.data, dyn_get_uint(vertexIndices.data, i)-1),
			add_slot_dyn_array(out_vertices)
		);
		copy_to_vec2(
			&dyn_get_vec2(temp_uvs.data, dyn_get_uint(uvIndices.data, i)-1),
			add_slot_dyn_array(out_uvs)
		);
		copy_to_vec3(
			&dyn_get_vec3(temp_normals.data, dyn_get_uint(normalIndices.data, i)-1),
			add_slot_dyn_array(out_normals)
		);
	}
	return 1;
}
