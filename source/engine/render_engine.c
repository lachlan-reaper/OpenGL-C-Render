#include "render_engine.h"

static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

render_engine_struct* initialiseRenderEngine(const int window_width, const int window_height) 
{
	{ // OpenGL Env. Initialisation
		glfwSetErrorCallback(error_callback);
		if (!glfwInit()) 
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
		// Open a window and create its OpenGL context
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
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(re_struct->window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(re_struct->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

int primeRenderEngine(render_engine_struct* re_struct)
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

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		// Cull triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
	}

	{ // Basic program and vertex buffers
		// Create and compile our GLSL program from the shaders
		re_struct->ids.programID = LoadShaders(re_struct->vertex_shader_path, re_struct->fragment_shader_path);

		glGenVertexArrays(1, &re_struct->ids.VertexArrayID);
		glBindVertexArray(re_struct->ids.VertexArrayID);
		glUseProgram(re_struct->ids.programID);
	}

	int res;
	res = re_struct->buffer_prime_function(re_struct);
	if (res != 0)
	{
		printf("Error on buffer prime function: %d\n", res);
		return 2;
	}

	// Get a handle for our "MVP" uniform
	re_struct->ids.MatrixID = glGetUniformLocation(re_struct->ids.programID, "MVP");
	re_struct->ids.ViewMatrixID = glGetUniformLocation(re_struct->ids.programID, "V");
	re_struct->ids.ModelMatrixID = glGetUniformLocation(re_struct->ids.programID, "M");

	{ // Texture handling
		// Load the texture
		re_struct->ids.Texture = loadDDS("./source/ext/textures/cubeuvmap.DDS"); // TODO: REMOVE LITERAL
		
		// Get a handle for our "myTextureSampler" uniform
		re_struct->ids.TextureID  = glGetUniformLocation(re_struct->ids.programID, "myTextureSampler");
	}

	loadObjectToModel(&(re_struct->model), "./source/ext/objects/suzanne.obj");

	{ // Binding buffers
		glGenBuffers(1, &re_struct->ids.vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, re_struct->ids.vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, re_struct->model.indexed_vertices.current_size * sizeof(vector3), &dyn_get_vec3(re_struct->model.indexed_vertices.data, 0), GL_STATIC_DRAW);

		glGenBuffers(1, &re_struct->ids.uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, re_struct->ids.uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, re_struct->model.indexed_uvs.current_size * sizeof(vector2), &dyn_get_vec2(re_struct->model.indexed_uvs.data, 0), GL_STATIC_DRAW);

		glGenBuffers(1, &re_struct->ids.normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, re_struct->ids.normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, re_struct->model.indexed_normals.current_size * sizeof(vector3), &dyn_get_vec3(re_struct->model.indexed_normals.data, 0), GL_STATIC_DRAW);

		glGenBuffers(1, &re_struct->ids.indexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, re_struct->ids.indexbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, re_struct->model.indexes.current_size * sizeof(unsigned int), &dyn_get_uint(re_struct->model.indexes.data, 0), GL_STATIC_DRAW);
	}

	{ // Camera + lights
		// Get a handle for our "LightPosition" uniform
		re_struct->ids.LightID = glGetUniformLocation(re_struct->ids.programID, "LightPosition_worldspace");

		set_camera(&re_struct->camera, deg_to_rad(24.0f), deg_to_rad(-24.0f), 45.0f);
		set_camera_position(&re_struct->camera, -3, 3, -7);

		set_vec3(&re_struct->lightPos, 4, 4, 4);
	}

	return 0;
}

int drawRenderEngine(render_engine_struct* re_struct)
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our shader
	glUseProgram(re_struct->ids.programID);

	if (re_struct->buffer_draw_function == NULL)
	{
		printf("No provided buffer_draw_function\n");
		return 1;
	}

	re_struct->buffer_draw_function(re_struct);
	calc_camera_mvp(&re_struct->camera, re_struct->window_width, re_struct->window_height);

	glUniformMatrix4fv(re_struct->ids.MatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.MVP.arr, 0, 0));
	glUniformMatrix4fv(re_struct->ids.ModelMatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.model.arr, 0, 0));
	glUniformMatrix4fv(re_struct->ids.ViewMatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.view.arr, 0, 0));

	glUniform3f(re_struct->ids.LightID, get_vec3(re_struct->lightPos.arr, 0), get_vec3(re_struct->lightPos.arr, 1), get_vec3(re_struct->lightPos.arr, 2));

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, re_struct->ids.Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(re_struct->ids.TextureID, 0);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, re_struct->ids.vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : uvs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, re_struct->ids.uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, re_struct->ids.normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, re_struct->ids.indexbuffer);

	// Draw the triangle !
	glDrawElements(GL_TRIANGLES, re_struct->model.indexes.current_size, GL_UNSIGNED_INT, (void*)0); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Swap buffers
	glfwSwapBuffers(re_struct->window);
	glfwPollEvents();

	return 0;
}

int cleanupRenderEngine(render_engine_struct* re_struct)
{
	if (re_struct->clean_up_function != NULL)
	{
		int res;
		res = re_struct->clean_up_function(re_struct);
		if (res != 0)
		{
			printf("Error on clean up function: %d\n", res);
			return 1;
		}
	}

	if (re_struct->buffer_clean_up_function == NULL)
	{
		printf("No provided buffer clean up function\n");
	}
	else 
	{
		int res;
		res = re_struct->buffer_clean_up_function(re_struct);
		if (res != 0)
		{
			printf("Error on buffer clean up function: %d\n", res);
			return 1;
		}
	}

	glDeleteVertexArrays(1, &re_struct->ids.VertexArrayID);

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

	if (re_struct->process_function == NULL)
	{
		do {
			if (drawRenderEngine(re_struct)) break;
		} // Check if the ESC key was pressed or the window was closed
		while( glfwGetKey(re_struct->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(re_struct->window) == 0 );
	}
	else
	{
		int res;
		do {
			res = re_struct->process_function(re_struct);
			if (res != 0)
			{
				printf("Error on process function: %d\n", res);
				cleanupRenderEngine(re_struct);
				return 3;
			}
			if (drawRenderEngine(re_struct)) break;
		} // Check if the ESC key was pressed or the window was closed
		while( glfwGetKey(re_struct->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(re_struct->window) == 0 );
	}
	
	return cleanupRenderEngine(re_struct);
}
