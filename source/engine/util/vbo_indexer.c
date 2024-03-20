#include "vbo_indexer.h"

#include <string.h> // for memcmp

struct PackedVertex{
	vector3 position;
	vector2 uv;
	vector3 normal;
};

#define COMP_PackedVertexs(PV_1, PV_2) memcmp((void*)(PV_1), (void*)(PV_2), sizeof(struct PackedVertex))

// TBU: update to binary search and insertion (for sorting)
const unsigned int find_PV_in_dyn_array(dyn_array* VertexToOutIndex, struct PackedVertex* packed)
{
	int i = 0;
	while (i < VertexToOutIndex->current_size && COMP_PackedVertexs(packed, get_dyn_array(VertexToOutIndex, i))) i++;
	return i;
}

int getSimilarVertexIndex_fast( 
	struct PackedVertex* packed, 
	dyn_array* VertexToOutIndex,
	unsigned int* result
){
	const unsigned int it = find_PV_in_dyn_array(VertexToOutIndex, packed);

	if (it == VertexToOutIndex->current_size) return 0;
	else{
		(*result) = it;
		return 1;
	}
}

void indexVBO(
	const dyn_array* in_vertices,
	const dyn_array* in_uvs,
	const dyn_array* in_normals,

	dyn_array* out_indices,
	dyn_array* out_vertices,
	dyn_array* out_uvs,
	dyn_array* out_normals
) {
	dyn_array VertexToOutIndex;
	set_dyn_array(&VertexToOutIndex, DYN_ARRAY_NO_TYPE);
	override_item_size_dyn_array(&VertexToOutIndex, sizeof(struct PackedVertex));

	// For each input vertex
	for (unsigned int i=0; i < in_vertices->current_size; i++)
	{
		struct PackedVertex packed = {
			dyn_get_vec3(in_vertices->data, i), 
			dyn_get_vec2(in_uvs->data, i), 
			dyn_get_vec3(in_normals->data, i)
		};
		
		// Try to find a similar vertex in out_XXXX
		unsigned int index;
		int found = getSimilarVertexIndex_fast(&packed, &VertexToOutIndex, &index);

		if (found) 
		{
			add_slot_dyn_array(out_indices);
			dyn_get_last_uint(out_indices) = index;
		}
		else
		{ // If not, it needs to be added in the output data.
			add_slot_dyn_array(out_vertices);
			add_slot_dyn_array(out_uvs);
			add_slot_dyn_array(out_normals);
			dyn_get_last_vec3(out_vertices) = dyn_get_vec3(in_vertices->data, i);
			dyn_get_last_vec2(out_uvs)      = dyn_get_vec2(in_uvs->data, i);
			dyn_get_last_vec3(out_normals)  = dyn_get_vec3(in_normals->data, i);

			add_slot_dyn_array(out_indices);
			dyn_get_last_uint(out_indices) = out_vertices->current_size - 1;

			// TBU: make binary insert
			add_slot_dyn_array(&VertexToOutIndex);
			*((struct PackedVertex*)dyn_get_last_void_ptr(&VertexToOutIndex)) = packed;
		}
	}
}