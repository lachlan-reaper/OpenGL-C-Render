#include "movement.h"

void process_camera_movement(GLFWwindow* window, Camera* camera, const VECTOR_FLT delta_time)
{
	VECTOR_FLT forward;
	VECTOR_FLT right;
	VECTOR_FLT up;
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) forward = FORWARD_SPEED * delta_time;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) forward = -FORWARD_SPEED * delta_time;
	else forward = 0;
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) right = RIGHT_SPEED * delta_time;
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) right = -RIGHT_SPEED * delta_time;
	else right = 0;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) up = UP_SPEED * delta_time;
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) up = -UP_SPEED * delta_time;
	else up = 0;

	update_position(camera, forward, right, up);

	double x_pos;
	double y_pos;

	glfwGetCursorPos(window, &x_pos, &y_pos);
	update_angles(camera, 
		(VECTOR_FLT)(WINDOW_WIDTH/2 - x_pos) * delta_time * CAMERA_SPEED, 
		(VECTOR_FLT)(WINDOW_HEIGHT/2 - y_pos) * delta_time * CAMERA_SPEED
	);
	glfwSetCursorPos(window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) update_FOV(camera, FOV_SPEED);
	else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) update_FOV(camera, -FOV_SPEED);
}
