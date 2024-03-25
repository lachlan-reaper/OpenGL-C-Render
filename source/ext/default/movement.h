#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../../engine/render_engine.h"
#include "../../engine/objects/camera.h"

#include <GLFW/glfw3.h>

#define FORWARD_SPEED 3
#define RIGHT_SPEED 3
#define UP_SPEED 3
#define CAMERA_SPEED 1
#define FOV_SPEED 0.1f

void process_camera_movement(render_engine_struct* const re_struct, const VECTOR_FLT delta_time);

#endif