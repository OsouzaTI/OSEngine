#ifndef MESH_H
#define MESH_H

#include <vector>
#include "vector.hpp"
#include "drawing.h"

typedef struct {
    int a;
    int b;
    int c;
    uint32_t color;
} face;


typedef struct {
    vect2<float> points[3];
    uint32_t color;
    float avg_depth;

} triangle;

typedef struct
{
    std::vector<vect3<float>> vertices;
    std::vector<face> faces;
    vect3<float> scale;
    vect3<float> rotation;
    vect3<float> translation;
} mesh_t;

class Mesh
{
    public:
        Mesh();
        ~Mesh();
        bool loaded;

        mesh_t mesh;
        std::vector<triangle> triangles_to_render;

        void load_obj_file_data(char* filename);        
        void update_mesh();
        void draw_mesh(Drawing& draw);

        void set_display(Display* display);

    private:        
        Display* display;
};

#endif