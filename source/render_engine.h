#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "../shaders/shader.h"
#include "./vectors/matrix_4x4.h"
#include "./vectors/vector4.h"
#include "./vectors/vector3.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

GLFWwindow* initialiseGLFWWindow();
void run(GLFWwindow* window);
void terminateWindow();

#endif