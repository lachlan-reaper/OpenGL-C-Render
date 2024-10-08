#include "render_engine.h"

static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

render_engine_struct* initialiseRenderEngine(const int window_width, const int window_height) 
{
	{ // OpenGL Env. Initialisation
		glfwSetErrorCallback(error_callback);
		if (glfwInit() != GL_TRUE)
		{
			fprintf(stdout, "[GFLW] failed to init!\n");
			exit(1);
		}

		glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	}

	render_engine_struct* re_struct = calloc(1, sizeof(render_engine_struct));
	{ // Window creation
		re_struct->window_width = window_width;
		re_struct->window_height = window_height;

		re_struct->window = glfwCreateWindow(re_struct->window_width, re_struct->window_height, "Renderer", NULL, NULL);
		if( re_struct->window == NULL )
		{
			fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
			glfwTerminate();
			return NULL;
		}
		glfwMakeContextCurrent(re_struct->window); // Initialize GLEW
		glewExperimental = 1; // Needed in core profile
		if (glewInit() != GLEW_OK) 
		{
			fprintf(stderr, "Failed to initialize GLEW\n");
			return NULL;
		}

		glfwSetInputMode(re_struct->window, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetInputMode(re_struct->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	{ // re_struct defaults
		re_struct->fragment_shader_path = NULL;
		re_struct->vertex_shader_path = NULL;

		set_vec4(&re_struct->default_bg, 0.0f, 0.0f, 0.0f, 0.0f);

		re_struct->prime_function = NULL;
		re_struct->process_function = NULL;
		re_struct->clean_up_function = NULL;

		re_struct->buffer_prime_function = NULL;
		re_struct->buffer_draw_function = NULL;
		re_struct->buffer_clean_up_function = NULL;
	}

	return re_struct;
}

int primeRenderEngine(render_engine_struct* const re_struct)
{
	if (re_struct->buffer_prime_function == NULL)
	{
		printf("No buffer_prime_function provided\n");
		return 1;
	}
	if (re_struct->fragment_shader_path == NULL)
	{
		printf("No fragment shader path provided\n");
		return 1;
	}
	if (re_struct->vertex_shader_path == NULL)
	{
		printf("No vertex shader path provided\n");
		return 1;
	}

	{ // OpenGL default settings		
		glClearColor(
			get_vec4(re_struct->default_bg.arr, 0), 
			get_vec4(re_struct->default_bg.arr, 1), 
			get_vec4(re_struct->default_bg.arr, 2), 
			get_vec4(re_struct->default_bg.arr, 3)
		);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}

	int res;
	res = re_struct->buffer_prime_function(re_struct);
	if (res != 0)
	{
		printf("Error on buffer prime function: %d\n", res);
		return 2;
	}

	return 0;
}

int drawRenderEngine(render_engine_struct* const re_struct)
{
	if (re_struct->buffer_draw_function == NULL)
	{
		printf("No provided buffer_draw_function\n");
		return 1;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(re_struct->programID);

	int res = re_struct->buffer_draw_function(re_struct);
	if (res != 0)
	{
		printf("Error on buffer draw function: %d\n", res);
		return 2;
	};

	glfwSwapBuffers(re_struct->window);
	glfwPollEvents();

	return 0;
}

int cleanupRenderEngine(render_engine_struct* const re_struct)
{
	if (re_struct->buffer_clean_up_function == NULL)
	{
		printf("No provided buffer clean up function\n");
		return 1;
	}

	if (re_struct->clean_up_function != NULL)
	{
		int res;
		res = re_struct->clean_up_function(re_struct);
		if (res != 0)
		{
			printf("Error on clean up function: %d\n", res);
			return 2;
		}
	}

	int res;
	res = re_struct->buffer_clean_up_function(re_struct);
	if (res != 0)
	{
		printf("Error on buffer clean up function: %d\n", res);
		return 2;
	}

	glDeleteProgram(re_struct->programID);

	glfwTerminate();
	return 0;
}

int run(render_engine_struct* re_struct) 
{
	int res = primeRenderEngine(re_struct);
	if (res != 0)
	{
		printf("Error on priming function: %d\n", res);
		return 1;
	}

	if (re_struct->prime_function != NULL)
	{
		int res;
		res = re_struct->prime_function(re_struct);
		if (res != 0)
		{
			printf("Error on priming function: %d\n", res);
			cleanupRenderEngine(re_struct);
			return 2;
		}
	}

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	if (re_struct->process_function == NULL)
	{
		do {
			{ // FPS Counter
				// Measure speed
				double currentTime = glfwGetTime();
				nbFrames++;
				if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
					// printf and reset
					printf("%f ms/frame\n", 1000.0/((double)nbFrames));
					nbFrames = 0;
					lastTime += 1.0;
				}
			}
			if (drawRenderEngine(re_struct)) break;
		}
		while( glfwGetKey(re_struct->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(re_struct->window) == 0 );
	}
	else
	{
		int res;
		do {
			{ // FPS Counter
				// Measure speed
				double currentTime = glfwGetTime();
				nbFrames++;
				if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
					// printf and reset
					printf("%f ms/frame\n", 1000.0/((double)nbFrames));
					nbFrames = 0;
					lastTime += 1.0;
				}
			}
			res = re_struct->process_function(re_struct);
			if (res != 0)
			{
				printf("Error on process function: %d\n", res);
				cleanupRenderEngine(re_struct);
				return 3;
			}
			if (drawRenderEngine(re_struct)) break;
		}
		while( glfwGetKey(re_struct->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(re_struct->window) == 0 );
	}
	
	return cleanupRenderEngine(re_struct);
}
