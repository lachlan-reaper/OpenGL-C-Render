#ifndef BASIC_MOVEMENT_H
#define BASIC_MOVEMENT_H

#include "../../engine/render_engine.h"
#include "../../engine/objects/camera.h"
#include "../../engine/default/basic_3d_functions.h"

#include <GLFW/glfw3.h>

#define FORWARD_SPEED 3
#define RIGHT_SPEED 3
#define UP_SPEED 3
#define CAMERA_SPEED 1
#define FOV_SPEED 0.1f

void process_camera_movement(render_engine_struct* const re_struct, const VECTOR_FLT delta_time);

int prime_function(render_engine_struct* const re_struct);
int process_function(render_engine_struct* const re_struct);
int clean_up_function(render_engine_struct* const re_struct);

void set_basic_movement(render_engine_struct* const re_struct);

#endif