#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif