#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "./vectors/vector2.h"
#include "./vectors/vector3.h"
#include "./vectors/vector4.h"
#include "./vectors/matrix_4x4.h"
#include "./vectors/dyn_array.h"

#include "./shaders/shader.h"
#include "./textures/texture_loader.h"
#include "./objects/camera.h"
#include "./objects/object_loader.h"

#include "../ext/movement.h" // TODO: REMOVE

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

typedef struct render_engine_struct {
	GLFWwindow* window;
	Camera camera;

	GLuint VertexArrayID;
	GLuint programID;
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint TextureID;
	GLuint LightID;

	GLuint Texture; // TODO: TBU

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;

	dyn_array vertices;
	dyn_array uvs;
	dyn_array normals;

	vector3 lightPos;

	double current_time;
	double last_time;
} render_engine_struct;

render_engine_struct* initialiseRenderEngine();
void run(render_engine_struct* re_struct);
void terminateWindow();

#endif