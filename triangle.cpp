#include "triangle.h"
#include "display.h"

TriangleHelper::TriangleHelper()
{
}

TriangleHelper::~TriangleHelper()
{
}

float TriangleHelper::back_face_culling(int x0, int y0, int x1, int y1, int x2, int y2) {

    vect3<float> a(x0, y0, 0 );
    vect3<float> b( x1, y1, 0 );
    vect3<float> c( x2, y2 ,0 );

    //subtract vector A B
    vect3<float> ab = vsubvect(b, a);
    vect3<float> ac = vsubvect(c, a);
    vnormalize(&ab);
    vnormalize(&ac);

    // Cross product 
    vect3<float> normal = vcrossvect(ab, ac);
    vnormalize(&normal);

    vect3<float> origin{ 0, 0 ,0 };
    vect3<float> camera_ray = vsubvect(origin, a);
    float dot_normal_camera = vdotvect(normal, camera_ray);

    return dot_normal_camera;
}

vect3<float> TriangleHelper::normal_light_direction(int x0, int y0, int x1, int y1, int x2, int y2)
{
    vect3<float> a( x0, y0, 0 );
    vect3<float> b( x1, y1, 0 );
    vect3<float> c( x2, y2, 0 );

    //subtract vector A B
    vect3<float> ab = vsubvect(b, a);
    vect3<float> ac = vsubvect(c, a);
    vnormalize(&ab);
    vnormalize(&ac);

    // Cross product 
    vect3<float> normal = vcrossvect(ab, ac);
    vnormalize(&normal);

    return normal;
}

vect3<float> TriangleHelper::barycentric_weights(
    int x, int y,
    int x0, int y0, // a
    int x1, int y1, // b
    int x2, int y2) // c
{
    vect2<int> ab{ x1 - x0, y1 - y0 };
    vect2<int> bc{ x2 - x1, y2 - y1 };
    vect2<int> ac{ x2 - x0, y2 - y0 };
    vect2<int> ap{ x - x0, y - y0 };
    vect2<int> bp{ x - x1, y - y1 };

    // calculate the area of the full triangle ABC using cross product ( area of paralelogram )
    float area_triangle_abc = (ab.x * ac.y - ab.y * ac.x);
    
    // Weight alpha is the area of subtriangle BCP divided by the area of the full triangle ABC
    float alpha = (bc.x * bp.y - bp.x * bc.y) / area_triangle_abc;

    // Weight beta is the area of subtriangle ACP divided by the area of the full triangle ABC
    float beta = (ap.x * ac.y - ac.x * ap.y) / area_triangle_abc;

    // Weight gamma is easily found since barycentric coordinates alwais add up to 1
    float gamma = 1 - alpha - beta;

    // Weights of alpha, beta and gamma inside a vector 3
    return vect3<float>{ alpha, beta, gamma };

}

void TriangleHelper::quick_sort_avg_depth(triangle* triangles_to_render, int inicio_vetor, int fim_vetor)
{
    int time_old = SDL_GetTicks();
    //Sort the triangles to render by their avg_depth [QUICK SORT]    
    int i = inicio_vetor;
    int j = fim_vetor - 1;
    triangle pivo = triangles_to_render[(i + j) / 2];
    while (i <= j) {
        while (triangles_to_render[i].avg_depth > pivo.avg_depth && i < fim_vetor) {
            i++;
        }
        while (triangles_to_render[j].avg_depth < pivo.avg_depth && j > inicio_vetor) {
            j--;
        }

        if (i <= j) {
            triangle aux = triangles_to_render[i];
            triangles_to_render[i] = triangles_to_render[j];
            triangles_to_render[j] = aux;
            i++;
            j--;
        }
    }
    if (j > inicio_vetor) {
        quick_sort_avg_depth(triangles_to_render, inicio_vetor, j+1);
    }
    if (i < fim_vetor) {
        quick_sort_avg_depth(triangles_to_render, i, fim_vetor);
    }    
    //std::cout << "Foram ordenados " << triangles_to_render.size()-1 << " triangulos em " << SDL_GetTicks() - time_old << "ms\n";
}
