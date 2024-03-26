#include "sprite.h"

static inline void recalc_instance_out_matrix(Sprite* const sprite, const SPRITE_INST_ID_TYPE instance_id)
{
	struct sprite_matrix* const sprite_mat = (struct sprite_matrix*) dyn_get_void_ptr(&sprite->instances, instance_id);
	matrix_3x3* out_matrix = &dyn_get_3x3(sprite->instances_sprite_matrix.data, instance_id);

	set_rotate_mat3x3(out_matrix, sprite_mat->rotation);

	get_3x3(out_matrix->arr, 0, 0) *= get_vec2(sprite_mat->xy_scale.arr, 0);
	get_3x3(out_matrix->arr, 1, 1) *= get_vec2(sprite_mat->xy_scale.arr, 1);

	get_3x3(out_matrix->arr, 2, 0) += get_vec3(sprite_mat->xyz_coords.arr, 0);
	get_3x3(out_matrix->arr, 2, 1) += get_vec3(sprite_mat->xyz_coords.arr, 1);
	get_3x3(out_matrix->arr, 2, 2) += get_vec3(sprite_mat->xyz_coords.arr, 2);
}

Sprite* newSprite(void)
{
	Sprite* sprite = calloc(1, sizeof(Sprite));
	initialiseSprite(sprite);
	return sprite;
}

void initialiseSprite(Sprite* const sprite)
{
	set_dyn_array(&sprite->instances, DYN_ARRAY_NO_TYPE);
	override_item_size_dyn_array(&sprite->instances, sizeof(struct sprite_matrix));
	set_dyn_array(&sprite->instances_sprite_matrix, DYN_ARRAY_MATRIX_3X3_TYPE);

	sprite->vertexbufferID = 0;
	sprite->Texture = 0;
}

void loadTextureToSprite(Sprite* const sprite, const char* path)
{
	sprite->Texture = loadDDS(path);
}

SPRITE_INST_ID_TYPE addSpriteInstance(Sprite* const sprite, const vector3 coords, const vector2 scale, const VECTOR_FLT rotation)
{
	add_slot_dyn_array(&sprite->instances);
	add_slot_dyn_array(&sprite->instances_sprite_matrix);
	((struct sprite_matrix*) dyn_get_last_void_ptr(&sprite->instances))->xyz_coords = coords;
	((struct sprite_matrix*) dyn_get_last_void_ptr(&sprite->instances))->xy_scale = scale;
	((struct sprite_matrix*) dyn_get_last_void_ptr(&sprite->instances))->rotation = rotation;

	const SPRITE_INST_ID_TYPE instance_id = sprite->instances.current_size - 1;

	recalc_instance_out_matrix(sprite, instance_id);

	return instance_id;
}

void clean_sprite(Sprite* const sprite)
{
	clean_dyn_array(&sprite->instances);
	clean_dyn_array(&sprite->instances_sprite_matrix);

	glDeleteBuffers(1, &sprite->vertexbufferID);
	glDeleteTextures(1, &sprite->Texture);
}
