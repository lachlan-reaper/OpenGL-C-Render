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

		set_dyn_array(&re_struct->models, DYN_ARRAY_NO_TYPE);
		override_item_size_dyn_array(&re_struct->models, sizeof(Model));
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

	{ // Basic program and vertex buffers
		re_struct->ids.programID = LoadShaders(re_struct->vertex_shader_path, re_struct->fragment_shader_path);

		glGenVertexArrays(1, &re_struct->ids.VertexArrayID);
		glBindVertexArray(re_struct->ids.VertexArrayID);
		glUseProgram(re_struct->ids.programID);

		re_struct->ids.textureHandle = glGetUniformLocation(re_struct->ids.programID, "myTextureSampler");
	}

	int res;
	res = re_struct->buffer_prime_function(re_struct);
	if (res != 0)
	{
		printf("Error on buffer prime function: %d\n", res);
		return 2;
	}

	re_struct->ids.VPMatrixID = glGetUniformLocation(re_struct->ids.programID, "VP");
	re_struct->ids.ViewMatrixID = glGetUniformLocation(re_struct->ids.programID, "V");
	re_struct->ids.InstanceModelArrID = glGetUniformLocation(re_struct->ids.programID, "instance_M");

	{ // Camera and lights initialisation
		re_struct->ids.LightID = glGetUniformLocation(re_struct->ids.programID, "LightPosition_worldspace");

		set_camera(&re_struct->camera, deg_to_rad(24.0f), deg_to_rad(-24.0f), 45.0f);
		set_camera_position(&re_struct->camera, -3, 3, -7);

		set_vec3(&re_struct->lightPos, 4, 4, 4);
	}

	return 0;
}

static void bindModel(const render_engine_struct* const re_struct, const Model* const model)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->Texture);
	glUniform1i(re_struct->ids.textureHandle, 0);

	glBindBuffer(GL_ARRAY_BUFFER, model->vertexbufferID);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, model->uvbufferID);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, model->normalbufferID);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indexbufferID);
}

static void bindInstancesAndDraw(const render_engine_struct* const re_struct, const Model* const model, const int size, const int instance_base)
{
	glUniformMatrix4fv(
		re_struct->ids.InstanceModelArrID, 
		size,
		GL_FALSE, 
		&get_4x4(dyn_get_4x4(model->instances_model_matrix.data, instance_base).arr, 0, 0)
	);

	glDrawElementsInstanced(
		GL_TRIANGLES, 
		model->indexes.current_size, 
		GL_UNSIGNED_INT, 
		(void*)0,
		model->instances_model_matrix.current_size
	);
}

int drawRenderEngine(render_engine_struct* const re_struct)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(re_struct->ids.programID);

	if (re_struct->buffer_draw_function == NULL)
	{
		printf("No provided buffer_draw_function\n");
		return 1;
	}

	re_struct->buffer_draw_function(re_struct);
	calc_camera_vp(&re_struct->camera, re_struct->window_width, re_struct->window_height);

	glUniformMatrix4fv(re_struct->ids.VPMatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.VP.arr, 0, 0));
	glUniformMatrix4fv(re_struct->ids.ViewMatrixID, 1, GL_FALSE, &get_4x4(re_struct->camera.view.arr, 0, 0));

	glUniform3f(re_struct->ids.LightID, get_vec3(re_struct->lightPos.arr, 0), get_vec3(re_struct->lightPos.arr, 1), get_vec3(re_struct->lightPos.arr, 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (int i = 0; i < re_struct->models.current_size; i++)
	{
		Model* const model = ((Model*)dyn_get_void_ptr(&re_struct->models, i));
		bindModel(re_struct, model);

		for (int batch_base = 0; batch_base < model->instances_model_matrix.current_size; batch_base += MAX_INSTANCES_BATCH)
		{
			bindInstancesAndDraw(
				re_struct, 
				model, 
				min(model->instances_model_matrix.current_size - batch_base, MAX_INSTANCES_BATCH), 
				batch_base
			);
		}
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glfwSwapBuffers(re_struct->window);
	glfwPollEvents();

	return 0;
}

MODEL_ID_TYPE addModel(render_engine_struct* const re_struct, const char* obj_path, const char* texture_path)
{
Model* const model = add_slot_dyn_array(&(re_struct->models));
initialiseModel(model);
loadObjectToModel(model, obj_path);
loadTextureToModel(model, texture_path);

{ // Generate VBO buffers
	glGenBuffers(1, &model->vertexbufferID);
	glBindBuffer(GL_ARRAY_BUFFER, model->vertexbufferID);
	glBufferData(GL_ARRAY_BUFFER, model->indexed_vertices.current_size * sizeof(vector3), &dyn_get_vec3(model->indexed_vertices.data, 0), GL_STATIC_DRAW);

	glGenBuffers(1, &model->uvbufferID);
	glBindBuffer(GL_ARRAY_BUFFER, model->uvbufferID);
	glBufferData(GL_ARRAY_BUFFER, model->indexed_uvs.current_size * sizeof(vector2), &dyn_get_vec2(model->indexed_uvs.data, 0), GL_STATIC_DRAW);

	glGenBuffers(1, &model->normalbufferID);
	glBindBuffer(GL_ARRAY_BUFFER, model->normalbufferID);
	glBufferData(GL_ARRAY_BUFFER, model->indexed_normals.current_size * sizeof(vector3), &dyn_get_vec3(model->indexed_normals.data, 0), GL_STATIC_DRAW);

	glGenBuffers(1, &model->indexbufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indexbufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indexes.current_size * sizeof(unsigned int), &dyn_get_uint(model->indexes.data, 0), GL_STATIC_DRAW);
}

return re_struct->models.current_size - 1;
}

/*
Returns instance id.
*/
MODEL_INST_ID_TYPE add_instance_of_model(render_engine_struct* const re_struct, const MODEL_ID_TYPE model_id, const vector3 coords, const vector3 scale, const vector3 rotation)
{
	return addModelInstance(dyn_get_void_ptr(&re_struct->models, model_id), coords, scale, rotation);
}

int cleanupRenderEngine(render_engine_struct* const re_struct)
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

	for (int i = 0; i < re_struct->models.current_size; i++) clean_model(dyn_get_void_ptr(&re_struct->models, i));

	clean_dyn_array(&re_struct->models);

	glDeleteVertexArrays(1, &re_struct->ids.VertexArrayID);

	glDeleteProgram(re_struct->ids.programID);

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
