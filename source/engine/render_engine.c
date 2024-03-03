#include "render_engine.h"

#ifndef VERTEX_SHADER_PATH
#define VERTEX_SHADER_PATH "./source/engine/shaders/VertexShader.vertexshader" // Default shader
#endif
#ifndef FRAGMENT_SHADER_PATH
#define FRAGMENT_SHADER_PATH "./source/engine/shaders/FragmentShader.fragmentshader" // Default shader
#endif

static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

render_engine_struct* initialiseRenderEngine() 
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
	re_struct->window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Renderer", NULL, NULL);
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

	return re_struct;
}

void run(render_engine_struct* re_struct) 
{
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	re_struct->programID = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(re_struct->programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(re_struct->programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(re_struct->programID, "M");

	// Load the texture
	GLuint Texture = loadDDS("./source/ext/textures/cubeuvmap.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(re_struct->programID, "myTextureSampler");

	// Read our .obj file
	set_dyn_array(&re_struct->vertices, DYN_ARRAY_VECTOR_3_TYPE);
	set_dyn_array(&re_struct->uvs, DYN_ARRAY_VECTOR_2_TYPE);
	set_dyn_array(&re_struct->normals, DYN_ARRAY_VECTOR_3_TYPE);
	loadOBJ("./source/ext/objects/suzanne.obj", &re_struct->vertices, &re_struct->uvs, &re_struct->normals); // TODO: REMOVE LITERAL

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, re_struct->vertices.current_size * sizeof(vector3), &dyn_get_vec3(re_struct->vertices.data, 0), GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, re_struct->uvs.current_size * sizeof(vector2), &dyn_get_vec2(re_struct->uvs.data, 0), GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
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

	// Get a handle for our "LightPosition" uniform
	glUseProgram(re_struct->programID);
	GLuint LightID = glGetUniformLocation(re_struct->programID, "LightPosition_worldspace");

	double current_time = glfwGetTime();
	double last_time;
	set_camera(&re_struct->camera, deg_to_rad(24.0f), deg_to_rad(-24.0f), 45.0f);
	set_camera_position(&re_struct->camera, -3, 3, -7);

	vector3 lightPos;
	set_vec3(&lightPos, 4, 4, 4);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(re_struct->programID);

		last_time = current_time;
		current_time = glfwGetTime();
		process_camera_movement(re_struct->window, &re_struct->camera, (VECTOR_FLT)(current_time - last_time));

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		calc_camera_mvp(&re_struct->camera);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.MVP.arr, 0, 0));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.model.arr, 0, 0));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.view.arr, 0, 0));

		glUniform3f(LightID, get_vec3(lightPos.arr, 0), get_vec3(lightPos.arr, 1), get_vec3(lightPos.arr, 2));

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
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

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(re_struct->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(re_struct->window) == 0 );
	
	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	// glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(re_struct->programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	clean_dyn_array(&re_struct->vertices);
	clean_dyn_array(&re_struct->uvs);
	clean_dyn_array(&re_struct->normals);
}

void terminateWindow()
{
	glfwTerminate();
}
