#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera {
	vect3<float> position;
	vect3<float> direction;
	vect3<float> forward_velocity;
	float yaw;
	float fov_x;
	float fov_y;
	float znear;
	float zfar;
	float aspect_x;
	float aspect_y;

	Camera() {};
	Camera(
		vect3<float> position,
		vect3<float> direction,
		float yaw,
		float fov_x,
		float fov_y,
		float znear,
		float zfar,
		float aspect_x,
		float aspect_y
	)
	{
		this->position = position;
		this->direction = direction;
		this->yaw = yaw;
		this->fov_x = fov_x;
		this->fov_y = fov_x;
		this->znear = znear;
		this->zfar = zfar;
		this->aspect_x = aspect_x;
		this->aspect_y = aspect_y;
	}

} Camera;


#endif // !CAMERA_H
