#ifndef MOVEMENT_H
#define MOVEMENT_H

// Include GLFW
#include <GLFW/glfw3.h>

#include "../objects/camera.h"

#define FORWARD_SPEED 3
#define RIGHT_SPEED 3
#define UP_SPEED 3
#define CAMERA_SPEED 1
#define FOV_SPEED 0.1f

void process_camera_movement(const GLFWwindow* window, Camera* camera, const VECTOR_FLT delta_time);

#endif