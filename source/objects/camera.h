#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>

#include "..\vectors\vector3.h"
#include "..\vectors\matrix_4x4.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

typedef struct Camera {
	vector3 position;
	vector3 direction;
	vector3 right;
	vector3 up;

	VECTOR_FLT horizontal_angle;
	VECTOR_FLT vertical_angle;
	VECTOR_FLT FOV;
} Camera;

void set_camera(Camera* camera, const VECTOR_FLT horizontal_angle, VECTOR_FLT vertical_angle, VECTOR_FLT FOV);
static inline void set_camera_position(Camera* camera, const VECTOR_FLT x, const VECTOR_FLT y, const VECTOR_FLT z)
{
	set_vec3(&camera->position, x, y, z);
}

/*
	To be called after any change to the angles
*/
static inline void update_vectors(Camera* camera)
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
static inline void update_angles(Camera* camera, const VECTOR_FLT delta_horz, const VECTOR_FLT delta_vert)
{
	camera->horizontal_angle += delta_horz;
	camera->vertical_angle += delta_vert;

	update_vectors(camera);
}

/*
	delta_??? = deltaTime * speed;
*/
static inline void update_position(Camera* camera, const VECTOR_FLT delta_forward, const VECTOR_FLT delta_right, const VECTOR_FLT delta_up)
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

static inline void update_FOV(Camera* camera, const VECTOR_FLT delta_FOV)
{
	camera->FOV += delta_FOV;
}

static inline void camera_look_at(const Camera* camera, matrix_4x4* view)
{
	vector3 look_at;
	copy_to_vec3(&camera->direction, &look_at);
	add_vec3s(&camera->position, &look_at);

	set_look_at_mat4x4(view, &camera->position, &look_at, &camera->up);
}

static inline void camera_perspective(const Camera* camera, matrix_4x4* perspective)
{
	set_perspective_mat4x4(perspective, deg_to_rad(camera->FOV), (VECTOR_FLT)WINDOW_WIDTH/(VECTOR_FLT)WINDOW_HEIGHT, 0.1f, 100.0f);
}

/*
	GLSL formatted MVP is stored in model
	TODO: include offsets to MODEL
*/
static inline void calc_camera_mvp(const Camera* camera, matrix_4x4* MVP)
{
	set_identity_mat4x4(MVP); // Model
	matrix_4x4 tmp_mat;
	camera_look_at(camera, &tmp_mat); // View
	cross_mat4x4_by_mat4x4(&tmp_mat, MVP);
	camera_perspective(camera, &tmp_mat); // Perspective
	cross_mat4x4_by_mat4x4(&tmp_mat, MVP);

	transpose_mat4x4(MVP); // TODO: REMOVE after changing 
}

#endif