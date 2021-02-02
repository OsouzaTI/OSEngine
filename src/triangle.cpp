#include "triangle.h"
#include "display.h"
#include "vector.h"

TriangleHelper::TriangleHelper()
{
    LOG("Construct the triangle helper");
}

TriangleHelper::~TriangleHelper()
{
    LOG("Deconstruct the triangle helper");
}

bool TriangleHelper::back_face_culling(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, vect3<float>* normal_outside) {

    /*
    ______            _   ______               _____       _ _ _
    | ___ \          | |  |  ___|             /  __ \     | | (_)
    | |_/ / __ _  ___| | _| |_ __ _  ___ ___  | /  \/_   _| | |_ _ __   __ _
    | ___ \/ _` |/ __| |/ /  _/ _` |/ __/ _ \ | |   | | | | | | | '_ \ / _` |
    | |_/ / (_| | (__|   <| || (_| | (_|  __/ | \__/\ |_| | | | | | | | (_| |
    \____/ \__,_|\___|_|\_\_| \__,_|\___\___|  \____/\__,_|_|_|_|_| |_|\__, |
                                                                        __/ |
                                                                       |___/
    */

    /*
    
        Essa função recebe como parâmetro, os vertices do triângulo usado 
        como objeto no teste

        vertice A :
            float x0;
            float y0;
            float z0;
        vertice B :
            float x1;
            float y1;
            float z1;
        vertice C :
            float x2;
            float y2;
            float z2;
    
        O parâmetro normal_outside é para que seja possível com apenas o calculo
        da normal da face com a camera em questão calcular a intensidade do flat shading

        
    */


    /* pontos em forma de vetores em R3 */
    vect3<float> vector_a(x0, y0, z0);
    vect3<float> vector_b(x1, y1, z1);
    vect3<float> vector_c(x2, y2, z2);

    // calculando de fato os vetores que ligam cada vertice
    vect3<float> vector_ab = vsubvect<float>(vector_b, vector_a);
    vect3<float> vector_ac = vsubvect<float>(vector_c, vector_a);
    // normalizando ( vetor diretor )
    vnormalize(&vector_ab);
    vnormalize(&vector_ac);

    // normal dessa face
    vect3<float> normal = vcrossvect(vector_ab, vector_ac);
    // normalizando novamente
    vnormalize(&normal);

    // atribuindo o ponteiro recebido como parâmetro
    *normal_outside = normal;

    // representa a origem do sistema de coordenadas
    vect3<float> origin{ 0, 0 ,0 };
    // disparando um raio da origem até um dos vertices
    vect3<float> camera_ray = vsubvect(origin, vector_a);
    // calculando o produto escalar dos vetores para determinar o ângulo entre eles
    float dot_normal_camera = vdotvect(normal, camera_ray);

    // definiindo se a face deve ser ocultada ou não
    if (dot_normal_camera < 0) {
        return true;
    }
    return false;
}

// TODO: determinar a necessidade desse metodo
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
    vect2<int> ab( x1 - x0, y1 - y0 );
    vect2<int> bc( x2 - x1, y2 - y1 );
    vect2<int> ac( x2 - x0, y2 - y0 );
    vect2<int> ap( x - x0, y - y0 );
    vect2<int> bp( x - x1, y - y1 );

    // calculate the area of the full triangle ABC using cross product ( area of paralelogram )
    float area_triangle_abc = vcrossvect(ab, ac);
    
    // Weight alpha is the area of subtriangle BCP divided by the area of the full triangle ABC
    float alpha = (bc.x * bp.y - bp.x * bc.y) / area_triangle_abc;

    // Weight beta is the area of subtriangle ACP divided by the area of the full triangle ABC
    float beta = (ap.x * ac.y - ac.x * ap.y) / area_triangle_abc;

    // Weight gamma is easily found since barycentric coordinates alwais add up to 1
    float gamma = 1 - alpha - beta;

    // Weights of alpha, beta and gamma inside a vector 3
    return vect3<float>{ alpha, beta, gamma };

}


/*
    Função criada para ordenar os triângulos a serem rasterizados por ordem de profundidade
*/
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
}

void TriangleHelper::triangles_from_polygon(polygon* polygon, triangle* triangles, int* number_of_triangles)
{
    for (int i = 0; i < polygon->num_vertices - 2; i++)
    {
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;

        triangles[i].points[0] = static_cast<vect4<float>>(polygon->vertices[index0]);
        triangles[i].points[1] = static_cast<vect4<float>>(polygon->vertices[index1]);
        triangles[i].points[2] = static_cast<vect4<float>>(polygon->vertices[index2]);

        triangles[i].textcoords[0] = polygon->textcoords[index0];
        triangles[i].textcoords[1] = polygon->textcoords[index1];
        triangles[i].textcoords[2] = polygon->textcoords[index2];

    }
    *number_of_triangles = polygon->num_vertices - 2;
}

void TriangleHelper::triangles_from_polygon(polygon4* polygon, triangle* triangles, int* number_of_triangles)
{
    for (int i = 0; i < polygon->num_vertices - 2; i++)
    {
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;

        triangles[i].points[0] = polygon->vertices[index0];
        triangles[i].points[1] = polygon->vertices[index1];
        triangles[i].points[2] = polygon->vertices[index2];

        triangles[i].textcoords[0] = polygon->textcoords[index0];
        triangles[i].textcoords[1] = polygon->textcoords[index1];
        triangles[i].textcoords[2] = polygon->textcoords[index2];

    }
    *number_of_triangles = polygon->num_vertices - 2;
}
