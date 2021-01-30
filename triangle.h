#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <vector>
#include "vector.h"
#include "mesh.h"

class TriangleHelper
{
public:
	TriangleHelper();
	~TriangleHelper();
    static float back_face_culling(int x0, int y0, int x1, int y1, int x2, int y2);
    static vect3<float> normal_light_direction(int x0, int y0, int x1, int y1, int x2, int y2);
	static vect3<float> barycentric_weights(int x, int y, int x0, int y0, int x1, int y1, int x2, int y2);
	static void quick_sort_avg_depth(triangle* triangles_to_render, int inicio_vetor, int fim_vetor);
private:

};

#endif // !TRIANGLE_H
