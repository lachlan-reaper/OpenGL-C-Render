#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct render_engine_struct render_engine_struct;

#include "./vectors/vector2.h"
#include "./vectors/vector3.h"
#include "./vectors/vector4.h"
#include "./vectors/matrix_4x4.h"

#include "./util/dyn_array.h"

typedef int (*re_process_function)(render_engine_struct* const);

struct render_engine_struct {
	GLFWwindow* window;

	int window_width;
	int window_height;

	char* vertex_shader_path;
	char* fragment_shader_path;
	GLuint programID;

	vector4 default_bg; // RGBA

	re_process_function prime_function; // return (0: success, !0: failure)
	re_process_function process_function; // return (0: success, !0: failure)
	re_process_function clean_up_function; // return (0: success, !0: failure)

	void* buffer_data;
	// TODO: REMOVE FUNC PTRS
	re_process_function buffer_prime_function; // return (0: success, !0: failure)
	re_process_function buffer_draw_function; // return (0: success, !0: failure)
	re_process_function buffer_clean_up_function; // return (0: success, !0: failure)
};

render_engine_struct* initialiseRenderEngine(const int window_width, const int window_height);
int primeRenderEngine(render_engine_struct* const re_struct);
int drawRenderEngine(render_engine_struct* const re_struct);
int cleanupRenderEngine(render_engine_struct* const re_struct);

int run(render_engine_struct* const re_struct);

#endif