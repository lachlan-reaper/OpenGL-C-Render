#include "movement.h"

void process_camera_movement(render_engine_struct* re_struct, const VECTOR_FLT delta_time)
{
	VECTOR_FLT forward;
	VECTOR_FLT right;
	VECTOR_FLT up;
	
	if (glfwGetKey(re_struct->window, GLFW_KEY_UP) == GLFW_PRESS) forward = FORWARD_SPEED * delta_time;
	else if (glfwGetKey(re_struct->window, GLFW_KEY_DOWN) == GLFW_PRESS) forward = -FORWARD_SPEED * delta_time;
	else forward = 0;
	
	if (glfwGetKey(re_struct->window, GLFW_KEY_RIGHT) == GLFW_PRESS) right = RIGHT_SPEED * delta_time;
	else if (glfwGetKey(re_struct->window, GLFW_KEY_LEFT) == GLFW_PRESS) right = -RIGHT_SPEED * delta_time;
	else right = 0;

	if (glfwGetKey(re_struct->window, GLFW_KEY_SPACE) == GLFW_PRESS) up = UP_SPEED * delta_time;
	else if (glfwGetKey(re_struct->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) up = -UP_SPEED * delta_time;
	else up = 0;

	update_position(&re_struct->camera, forward, right, up);

	double x_pos;
	double y_pos;

	glfwGetCursorPos(re_struct->window, &x_pos, &y_pos);
	update_angles(&re_struct->camera, 
		(VECTOR_FLT)(re_struct->window_width/2 - x_pos) * delta_time * CAMERA_SPEED, 
		(VECTOR_FLT)(re_struct->window_height/2 - y_pos) * delta_time * CAMERA_SPEED
	);
	glfwSetCursorPos(re_struct->window, re_struct->window_width/2, re_struct->window_height/2);

	if (glfwGetKey(re_struct->window, GLFW_KEY_KP_ADD) == GLFW_PRESS) update_FOV(&re_struct->camera, FOV_SPEED);
	else if (glfwGetKey(re_struct->window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) update_FOV(&re_struct->camera, -FOV_SPEED);
}
