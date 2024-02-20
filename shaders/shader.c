#include "shader.h"

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	FILE *fptr;
	// use appropriate location if you are using MacOS or Linux
	fptr = fopen(vertex_file_path, "r");
	if(fptr == NULL)
	{
		printf("Error!");
		return 0;
	}

	fseek(fptr, 0, SEEK_END); // seek to end of file
	long VertexShaderCodeSize = ftell(fptr); // get current file pointer
	fseek(fptr, 0, SEEK_SET); // seek back to beginning of file
	// proceed with allocating memory and reading the file

	char* VertexShaderCode = calloc(++VertexShaderCodeSize, sizeof(char));
	fread(VertexShaderCode, sizeof(char), VertexShaderCodeSize, fptr);

	fclose(fptr);

	// use appropriate location if you are using MacOS or Linux
	fptr = fopen(fragment_file_path, "r");
	if(fptr == NULL)
	{
		printf("Error!");
		return 0;
	}

	fseek(fptr, 0, SEEK_END); // seek to end of file
	long FragmentShaderCodeSize = ftell(fptr); // get current file pointer
	fseek(fptr, 0, SEEK_SET); // seek back to beginning of file
	// proceed with allocating memory and reading the file

	char* FragmentShaderCode = calloc(++FragmentShaderCodeSize, sizeof(char));
	fread(FragmentShaderCode, sizeof(char), FragmentShaderCodeSize, fptr);

	fclose(fptr);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	const char* const vsptr = VertexShaderCode;
	glShaderSource(VertexShaderID, 1, &vsptr , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	char* VertexShaderErrorMessage;
	if ( InfoLogLength > 0 ){
		VertexShaderErrorMessage = calloc((InfoLogLength+1), sizeof(char));
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, VertexShaderErrorMessage);
		printf("%s\n", VertexShaderErrorMessage);
	}
	else {
		VertexShaderErrorMessage = NULL;
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	const char* const fsptr = FragmentShaderCode;
	glShaderSource(FragmentShaderID, 1, &fsptr , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	char* FragmentShaderErrorMessage;
	if ( InfoLogLength > 0 ){
		FragmentShaderErrorMessage = calloc((InfoLogLength+1), sizeof(char));
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, FragmentShaderErrorMessage);
		printf("%s\n", FragmentShaderErrorMessage);
	}
	else {
		FragmentShaderErrorMessage = NULL;
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	char* ProgramErrorMessage;
	if ( InfoLogLength > 0 ){
		ProgramErrorMessage = calloc((InfoLogLength+1), sizeof(char));
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage);
		printf("%s\n", ProgramErrorMessage);
	}
	else
	{
		ProgramErrorMessage = NULL;
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	free(VertexShaderCode);
	free(FragmentShaderCode);
	free(VertexShaderErrorMessage);
	free(FragmentShaderErrorMessage);
	free(ProgramErrorMessage);

	return ProgramID;
}