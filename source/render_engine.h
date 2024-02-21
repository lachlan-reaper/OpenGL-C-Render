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

GLFWwindow* initialiseGLFWWindow();
void run(GLFWwindow* window);
void terminateWindow();

#endif