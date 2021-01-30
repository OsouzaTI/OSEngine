#ifndef CLIPPING_H
#define CLIPPING_H

#include "vector.h"
#include "macros.h"

#define NUM_FRUSTUM_PLANES 6

enum FRUSTUM_PLANE {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	NEAR,
	FAR
};

typedef struct {
	vect3<float> point;
	vect3<float> normal;
} plane;

class FrustumPlane
{
	public:
		FrustumPlane();
		~FrustumPlane();
		static FrustumPlane* get_instance();

		void init_frustum_planes(float fov, float znear, float zfar);
		plane* get_frustum_planes();
	private:
		static FrustumPlane* instance;
		plane frustrum_planes[NUM_FRUSTUM_PLANES];

};

#endif // !CLIPPING_H
