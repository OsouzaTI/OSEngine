#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera {
	vect3<float> position;
	vect3<float> direction;
	vect3<float> forward_velocity;
	float yaw;
	float fov;
	float znear;
	float zfar;
	float aspect;

	Camera() {};
	Camera(
		vect3<float> position,
		vect3<float> direction,
		float yaw,
		float fov, float znear, float zfar, float aspect)
	{
		this->position = position;
		this->direction = direction;
		this->yaw = yaw;
		this->fov = fov;
		this->znear = znear;
		this->zfar = zfar;
		this->aspect = aspect;
	}

} Camera;


#endif // !CAMERA_H
