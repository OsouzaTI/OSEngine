#ifndef POLYGON_H
#define POLYGON_H

#define MAX_NUM_POLYGONS 10

#include "vector.h"
#include "macros.h"
#include "clipping.h"

struct UVtext {
	float u;
	float v;

	UVtext clone() {
		return UVtext{ u, v };
	}

};

typedef struct UVtext UVtext;


inline UVtext UVclone(UVtext* texture) {
	return UVtext{ texture->u, texture->v };
}

typedef struct {
	vect3<float> vertices[MAX_NUM_POLYGONS];
	UVtext textcoords[MAX_NUM_POLYGONS];
	int num_vertices;
} polygon;

class PolygonHelper
{
	public:
		PolygonHelper();
		~PolygonHelper();

		static void clip_polygon(polygon* polygon, FrustumPlane* frustum);
		static void clip_polygon_against_plane(
			polygon* polygon,
			FrustumPlane* frustum,
			FRUSTUM_PLANE frustum_plane
		);
		static polygon create_polygon_from_triangle(
			vect3<float> v0,
			vect3<float> v1,
			vect3<float> v2,
			UVtext t0,
			UVtext t1,
			UVtext t2
		);
	private:

};

#endif // !POLYGON_H
