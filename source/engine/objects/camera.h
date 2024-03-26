#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>

#include "../vectors/vector3.h"
#include "../vectors/matrix_4x4.h"

typedef struct Camera {
	vector3 position;
	vector3 direction;
	vector3 right;
	vector3 up;

	VECTOR_FLT horizontal_angle;
	VECTOR_FLT vertical_angle;
	VECTOR_FLT FOV;

	matrix_4x4 view;
	matrix_4x4 perspective;
	matrix_4x4 VP;
} Camera;

void set_camera(Camera* const camera, const VECTOR_FLT horizontal_angle, const VECTOR_FLT vertical_angle, const VECTOR_FLT FOV);
static inline void set_camera_position(Camera* const camera, const VECTOR_FLT x, const VECTOR_FLT y, const VECTOR_FLT z)
{
	set_vec3(&camera->position, x, y, z);
}

/*
	To be called after any change to the angles
*/
static inline void update_vectors(Camera* const camera)
{
	set_vec3(&camera->direction,
		cos(camera->vertical_angle) * sin(camera->horizontal_angle),
		sin(camera->vertical_angle),
		cos(camera->vertical_angle) * cos(camera->horizontal_angle)
	);
	set_vec3(&camera->right,
		sin(camera->horizontal_angle - M_PI/2.0f),
		0,
		cos(camera->horizontal_angle - M_PI/2.0f)
	);

	copy_to_vec3(&camera->direction, &camera->up);
	cross_vec3_by_vec3(&camera->right, &camera->up);
}

/*
	delta_??? = distance * deltaTime * speed;
*/
static inline void update_angles(Camera* const camera, const VECTOR_FLT delta_horz, const VECTOR_FLT delta_vert)
{
	camera->horizontal_angle += delta_horz;
	camera->vertical_angle += delta_vert;

	update_vectors(camera);
}

/*
	delta_??? = deltaTime * speed;
*/
static inline void update_position(Camera* const camera, const VECTOR_FLT delta_forward, const VECTOR_FLT delta_right, const VECTOR_FLT delta_up)
{
	vector3 tmp_vect;

	copy_to_vec3(&camera->direction, &tmp_vect);
	scale_vec3_xyz(&tmp_vect, delta_forward);
	add_vec3s(&tmp_vect, &camera->position);

	copy_to_vec3(&camera->right, &tmp_vect);
	scale_vec3_xyz(&tmp_vect, delta_right);
	add_vec3s(&tmp_vect, &camera->position);

	copy_to_vec3(&camera->up, &tmp_vect);
	scale_vec3_xyz(&tmp_vect, delta_up);
	add_vec3s(&tmp_vect, &camera->position);
}

static inline void update_FOV(Camera* const camera, const VECTOR_FLT delta_FOV)
{
	camera->FOV += delta_FOV;
}

static inline void camera_look_at(const Camera* const camera, matrix_4x4* view)
{
	vector3 look_at;
	copy_to_vec3(&camera->direction, &look_at);
	add_vec3s(&camera->position, &look_at);

	set_look_at_mat4x4(view, &camera->position, &look_at, &camera->up);
}

static inline void camera_perspective(const Camera* const camera, matrix_4x4* perspective, const int window_width, const int window_height)
{
	set_perspective_mat4x4(perspective, deg_to_rad(camera->FOV), (VECTOR_FLT)window_width/(VECTOR_FLT)window_height, 0.1f, 100.0f);
}

/*
	GLSL formatted VP is stored in VP; V and P also updated
*/
static inline void calc_camera_3d_vp(Camera* const camera, const int window_width, const int window_height)
{
	camera_look_at(camera, &camera->view); // View
	copy_to_mat4x4(&camera->view, &camera->VP);
	
	camera_perspective(camera, &camera->perspective, window_width, window_height); // Perspective
	cross_mat4x4_by_mat4x4(&camera->perspective, &camera->VP);
}

#endif