#include "render_engine.h"

static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

GLFWwindow* initialiseGLFWWindow() 
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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
	window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Renderer", NULL, NULL);
	if( window == NULL )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = 1; // Needed in core profile
	if (glewInit() != GLEW_OK) 
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return NULL;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	return window;
}

void run(GLFWwindow* window) 
{
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "./shaders/SimpleVertexShader.vertexshader", "./shaders/SimpleFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	vector3 location;
	vector3 fixation;
	vector3 rotation;
	set_vec3(&location, 4, 3, 3);
	set_vec3(&fixation, 0, 0, 0);
	set_vec3(&rotation, 0, 1, 0);

	matrix_4sq MVP;
	set_identity_mat4sq(&MVP); // Model
	transform_look_at_mat4sq(&MVP, &location, &fixation, &rotation); // View
	transform_perspective_mat4sq(&MVP, deg_to_rad(45.0f), (VECTOR_FLT)WINDOW_WIDTH/(VECTOR_FLT)WINDOW_HEIGHT, 0.1f, 100.0f); // Perspective
	for (int i = 0; i < 4; i++)
	{
		printf("\n");
		for (int x = 0; x < 4; x++)
		{
			printf("%f ", MVP.arr[4*i + x]);
		}
	}
	// MVP BUILT
	matrix_4sq MVP_clmn_ord;
	transpose_mat4sq(&MVP, &MVP_clmn_ord);

	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	printf("\n");
	vector4 tmp;
	for (int i = 0; i < 3; i++)
	{
		set_vec4(&tmp, g_vertex_buffer_data[3*i], g_vertex_buffer_data[3*i+1], g_vertex_buffer_data[3*i+2], 1);
		cross_mat4sq_by_vec4(&MVP, &tmp);
		printf("\n");
		for (int x = 0; x < 4; x++)
		{
			printf("%f ", tmp.arr[x]);
		}
	}

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP_clmn_ord.arr[0]);

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

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 );
	
	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
}

void terminateWindow()
{
	glfwTerminate();
}
