#include "render_engine.h"

static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

render_engine_struct* initialiseRenderEngine(const int window_width, const int window_height) 
{
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

	// Open a window and create its OpenGL context
	render_engine_struct* re_struct = calloc(1, sizeof(render_engine_struct));
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

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(re_struct->window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(re_struct->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	re_struct->fragment_shader_path = NULL;
	re_struct->vertex_shader_path = NULL;

	set_vec4(&re_struct->default_bg, 0.0f, 0.0f, 0.0f, 0.0f);

	re_struct->prime_function = NULL;
	re_struct->process_function = NULL;
	re_struct->clean_up_function = NULL;

	re_struct->buffer_prime_function = NULL;
	re_struct->buffer_draw_function = NULL;
	re_struct->buffer_clean_up_function = NULL;

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

	// Create and compile our GLSL program from the shaders
	re_struct->programID = LoadShaders(re_struct->vertex_shader_path, re_struct->fragment_shader_path);

	glGenVertexArrays(1, &re_struct->VertexArrayID);
	glBindVertexArray(re_struct->VertexArrayID);
	glUseProgram(re_struct->programID);

	int res;
	res = re_struct->buffer_prime_function(re_struct);
	if (res != 0)
	{
		printf("Error on buffer prime function: %d\n", res);
		return 2;
	}
	// Get a handle for our "MVP" uniform
	re_struct->MatrixID = glGetUniformLocation(re_struct->programID, "MVP");
	re_struct->ViewMatrixID = glGetUniformLocation(re_struct->programID, "V");
	re_struct->ModelMatrixID = glGetUniformLocation(re_struct->programID, "M");

	// Load the texture
	re_struct->Texture = loadDDS("./source/ext/textures/cubeuvmap.DDS"); // TODO: REMOVE LITERAL
	
	// Get a handle for our "myTextureSampler" uniform
	re_struct->TextureID  = glGetUniformLocation(re_struct->programID, "myTextureSampler");

	glGenBuffers(1, &re_struct->vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, re_struct->vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, re_struct->vertices.current_size * sizeof(vector3), &dyn_get_vec3(re_struct->vertices.data, 0), GL_STATIC_DRAW);

	glGenBuffers(1, &re_struct->uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, re_struct->uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, re_struct->uvs.current_size * sizeof(vector2), &dyn_get_vec2(re_struct->uvs.data, 0), GL_STATIC_DRAW);

	glGenBuffers(1, &re_struct->normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, re_struct->normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, re_struct->normals.current_size * sizeof(vector3), &dyn_get_vec3(re_struct->normals.data, 0), GL_STATIC_DRAW);
	
	/* Colour
	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	*/

	// Read our .obj file
	set_dyn_array(&re_struct->vertices, DYN_ARRAY_VECTOR_3_TYPE);
	set_dyn_array(&re_struct->uvs, DYN_ARRAY_VECTOR_2_TYPE);
	set_dyn_array(&re_struct->normals, DYN_ARRAY_VECTOR_3_TYPE);
	loadOBJ("./source/ext/objects/suzanne.obj", &re_struct->vertices, &re_struct->uvs, &re_struct->normals); // TODO: REMOVE LITERAL

	// Get a handle for our "LightPosition" uniform
	re_struct->LightID = glGetUniformLocation(re_struct->programID, "LightPosition_worldspace");

	set_camera(&re_struct->camera, deg_to_rad(24.0f), deg_to_rad(-24.0f), 45.0f);
	set_camera_position(&re_struct->camera, -3, 3, -7);

	set_vec3(&re_struct->lightPos, 4, 4, 4);

	return 0;
}

int drawRenderEngine(render_engine_struct* re_struct)
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our shader
	glUseProgram(re_struct->programID);

	if (re_struct->buffer_draw_function == NULL)
	{
		printf("No provided buffer_draw_function\n");
		return 1;
	}

	re_struct->buffer_draw_function(re_struct);
	calc_camera_mvp(&re_struct->camera, re_struct->window_width, re_struct->window_height);

	glUniformMatrix4fv(re_struct->MatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.MVP.arr, 0, 0));
	glUniformMatrix4fv(re_struct->ModelMatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.model.arr, 0, 0));
	glUniformMatrix4fv(re_struct->ViewMatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.view.arr, 0, 0));

	glUniform3f(re_struct->LightID, get_vec3(re_struct->lightPos.arr, 0), get_vec3(re_struct->lightPos.arr, 1), get_vec3(re_struct->lightPos.arr, 2));

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, re_struct->Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(re_struct->TextureID, 0);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, re_struct->vertexbuffer);
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
	glBindBuffer(GL_ARRAY_BUFFER, re_struct->uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	/* Colours
	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	*/

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, re_struct->normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, re_struct->vertices.current_size); // 3 indices starting at 0 -> 1 triangle

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

	glDeleteVertexArrays(1, &re_struct->VertexArrayID);

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
