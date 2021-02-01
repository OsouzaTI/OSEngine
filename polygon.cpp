#include "polygon.h"
#include "math.h"

PolygonHelper::PolygonHelper()
{
	LOG("Construct PolygonHelper");
}

PolygonHelper::~PolygonHelper()
{
	LOG("Deconstruct PolygonHelper");
}

void PolygonHelper::clip_polygon(polygon* polygon, FrustumPlane* frustum)
{
	clip_polygon_against_plane(polygon, frustum, FRUSTUM_PLANE::LEFT);
	clip_polygon_against_plane(polygon, frustum, FRUSTUM_PLANE::RIGHT);
	clip_polygon_against_plane(polygon, frustum, FRUSTUM_PLANE::TOP);
	clip_polygon_against_plane(polygon, frustum, FRUSTUM_PLANE::BOTTOM);
	clip_polygon_against_plane(polygon, frustum, FRUSTUM_PLANE::NEAR);
	//clip_polygon_against_plane(polygon, frustum, FRUSTUM_PLANE::FAR);
}

void PolygonHelper::clip_polygon_against_plane(
	polygon* polygon,
	FrustumPlane* frustum,
	FRUSTUM_PLANE frustum_plane
)
{
	
	vect3<float> plane_point = frustum->get_frustum_planes()[frustum_plane].point;
	vect3<float> plane_normal = frustum->get_frustum_planes()[frustum_plane].normal;

	vect3<float> inside_vertices[MAX_NUM_POLYGONS];
	UVtext inside_uvcoords[MAX_NUM_POLYGONS];
	int num_inside_vertices = 0;

	vect3<float>* current_vertex = &polygon->vertices[0];
	UVtext* current_textcoords = &polygon->textcoords[0];	

	vect3<float>* previous_vertex = &polygon->vertices[polygon->num_vertices - 1];
	UVtext* previous_textcoords = &polygon->textcoords[polygon->num_vertices - 1];

	float current_dot = 0;
	float previous_dot = vdotvect<float>(vsubvect(*previous_vertex, plane_point), plane_normal);

	while (current_vertex != &polygon->vertices[polygon->num_vertices]) {
		current_dot = vdotvect<float>(vsubvect(*current_vertex, plane_point), plane_normal);

		if (current_dot * previous_dot < 0) {
			float t_factor = previous_dot / (previous_dot - current_dot);
			vect3<float> intersection_point{
				Math::lerp<float>(previous_vertex->x, current_vertex->x, t_factor),
				Math::lerp<float>(previous_vertex->y, current_vertex->y, t_factor),
				Math::lerp<float>(previous_vertex->z, current_vertex->z, t_factor)
			};

			UVtext interpolated_UV{
				Math::lerp<float>(previous_textcoords->u, current_textcoords->u, t_factor),
				Math::lerp<float>(previous_textcoords->v, current_textcoords->v, t_factor)
			};

			inside_vertices[num_inside_vertices] = vptrclone(&intersection_point);
			inside_uvcoords[num_inside_vertices] = interpolated_UV.clone();;
			num_inside_vertices++;
		}

		if (current_dot > 0) {
			inside_vertices[num_inside_vertices] = vptrclone(current_vertex);
			inside_uvcoords[num_inside_vertices] = current_textcoords->clone();
			num_inside_vertices++;
		}

		previous_dot = current_dot;
		previous_vertex = current_vertex;
		previous_textcoords = current_textcoords;
		current_vertex++;
		current_textcoords++;
	}

	for (int i = 0; i < num_inside_vertices; i++)
	{
		polygon->vertices[i] = vptrclone(&inside_vertices[i]);
		polygon->textcoords[i] = inside_uvcoords[i].clone();
	}
	polygon->num_vertices = num_inside_vertices;
}

polygon PolygonHelper::create_polygon_from_triangle(
	vect3<float> v0,
	vect3<float> v1,
	vect3<float> v2,
	UVtext t0,
	UVtext t1,
	UVtext t2
)
{
	return polygon{
		{v0, v1, v2},
		{t0, t1, t2},
		3
	};
}

