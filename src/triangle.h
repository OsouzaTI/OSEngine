#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <vector>
#include "vector.h"
#include "polygon.h"

#define MAX_NUM_POLYGONS_TRIANGLES 10

typedef struct {
    int a;
    int b;
    int c;
    UVtext a_uv;
    UVtext b_uv;
    UVtext c_uv;
    uint32_t color;
} face;


typedef struct {
    vect4<float> points[3];
    UVtext textcoords[3];
    uint32_t color;
    float avg_depth;
} triangle;


class TriangleHelper
{
public:
	TriangleHelper();
	~TriangleHelper();
    static bool back_face_culling(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, vect3<float>* normal);
    static vect3<float> normal_light_direction(int x0, int y0, int x1, int y1, int x2, int y2);
	static vect3<float> barycentric_weights(int x, int y, int x0, int y0, int x1, int y1, int x2, int y2);
	static void quick_sort_avg_depth(triangle* triangles_to_render, int inicio_vetor, int fim_vetor);
    static void triangles_from_polygon(polygon* polygon, triangle* triangles, int* number_of_triangles);
    static void triangles_from_polygon(polygon4* polygon, triangle* triangles, int* number_of_triangles);
private:

};

#endif // !TRIANGLE_H
