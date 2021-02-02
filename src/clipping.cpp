#include "clipping.h"

FrustumPlane* FrustumPlane::instance = NULL;

FrustumPlane::FrustumPlane()
{
	LOG("Init the class FrustumPlane");
}

FrustumPlane::~FrustumPlane()
{
	LOG("Destruct the class FrustumPlane");
}

FrustumPlane* FrustumPlane::get_instance()
{
	if (FrustumPlane::instance == NULL){
		FrustumPlane::instance = new FrustumPlane();
	}
	return FrustumPlane::instance;
}

void FrustumPlane::init_frustum_planes(float fov_y, float fov_x, float znear, float zfar)
{
	float cos_half_fov_x = cos(fov_x / 2.0f);
	float sin_half_fov_x = sin(fov_x / 2.0f);
	
	float cos_half_fov_y = cos(fov_y / 2.0f);
	float sin_half_fov_y = sin(fov_y / 2.0f);


	frustrum_planes[FRUSTUM_PLANE::LEFT].point = vect3<float>(0, 0, 0);
	frustrum_planes[FRUSTUM_PLANE::LEFT].normal.x = cos_half_fov_x;
	frustrum_planes[FRUSTUM_PLANE::LEFT].normal.y = 0;
	frustrum_planes[FRUSTUM_PLANE::LEFT].normal.z = sin_half_fov_x;

	frustrum_planes[FRUSTUM_PLANE::RIGHT].point = vect3<float>(0, 0, 0);
	frustrum_planes[FRUSTUM_PLANE::RIGHT].normal.x = -cos_half_fov_x;
	frustrum_planes[FRUSTUM_PLANE::RIGHT].normal.y = 0;
	frustrum_planes[FRUSTUM_PLANE::RIGHT].normal.z = sin_half_fov_x;

	frustrum_planes[FRUSTUM_PLANE::TOP].point = vect3<float>(0, 0, 0);
	frustrum_planes[FRUSTUM_PLANE::TOP].normal.x = 0;
	frustrum_planes[FRUSTUM_PLANE::TOP].normal.y = -cos_half_fov_y;
	frustrum_planes[FRUSTUM_PLANE::TOP].normal.z = sin_half_fov_y;

	frustrum_planes[FRUSTUM_PLANE::BOTTOM].point = vect3<float>(0, 0, 0);
	frustrum_planes[FRUSTUM_PLANE::BOTTOM].normal.x = 0;
	frustrum_planes[FRUSTUM_PLANE::BOTTOM].normal.y = cos_half_fov_y;
	frustrum_planes[FRUSTUM_PLANE::BOTTOM].normal.z = sin_half_fov_y;

	frustrum_planes[FRUSTUM_PLANE::NEAR].point = vect3<float>(0, 0, znear);
	frustrum_planes[FRUSTUM_PLANE::NEAR].normal.x = 0;
	frustrum_planes[FRUSTUM_PLANE::NEAR].normal.y = 0;
	frustrum_planes[FRUSTUM_PLANE::NEAR].normal.z = 1;

	frustrum_planes[FRUSTUM_PLANE::FAR].point = vect3<float>(0, 0, zfar);
	frustrum_planes[FRUSTUM_PLANE::FAR].normal.x = 0;
	frustrum_planes[FRUSTUM_PLANE::FAR].normal.y = 0;
	frustrum_planes[FRUSTUM_PLANE::FAR].normal.z = -1;

}

plane* FrustumPlane::get_frustum_planes()
{
	return frustrum_planes;
}
