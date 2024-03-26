#ifndef SPRITE_H
#define SPRITE_H

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

#include "../vectors/vector2.h"
#include "../vectors/vector3.h"
#include "../vectors/matrix_3x3.h"

#include "../util/dyn_array.h"
#include "../util/object_loader.h"
#include "../util/texture_loader.h"
#include "../util/vbo_indexer.h"

#define SPRITE_INST_ID_TYPE unsigned int

struct sprite_matrix {
	vector3 xyz_coords;
	vector2 xy_scale;
	VECTOR_FLT rotation;
};

typedef struct Sprite {
	dyn_array instances; // type: struct sprite_matrix
	dyn_array instances_sprite_matrix; // type: matrix_3x3

	GLuint vertexbufferID;
	GLuint Texture;
} Sprite;

Sprite* newSprite(void);
void initialiseSprite(Sprite* const sprite);
void loadTextureToSprite(Sprite* const sprite, const char* path);

SPRITE_INST_ID_TYPE addSpriteInstance(Sprite* const sprite, const vector3 coords, const vector2 scale, const VECTOR_FLT rotation);

void clean_sprite(Sprite* const sprite);

#endif