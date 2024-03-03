#include "camera.h"

void set_camera(Camera* camera, const VECTOR_FLT horizontal_angle, const VECTOR_FLT vertical_angle, const VECTOR_FLT FOV)
{
	set_vec3(&camera->position, 0, 0, 0);

	camera->horizontal_angle = horizontal_angle;
	camera->vertical_angle = vertical_angle;
	camera->FOV = FOV;

	update_vectors(camera);
}
