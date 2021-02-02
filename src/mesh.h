#ifndef MESH_H
#define MESH_H

#define MAX_TRIANGLES_PER_MESH 40000

#include <vector>
#include "vector.h"
#include "matrix.h"
#include "drawing.h"
#include "texture.h"
#include "triangle.h"

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
        OSTexture texture;
        triangle* triangles_to_render;
        unsigned int number_of_triangles_to_render = 0;

        void load_cube_object();
        void load_obj_file_data(const char* filename);  
        void load_obj_file_dataV2(const char* filename);        

        void set_display(Display* display);
        void set_color(uint32_t color);
        uint32_t get_color();

        void set_translation(vect3<float> translation);
        void set_scale(vect3<float> scale);
        void set_rotate(vect3<float> rotation);
        void set_shearXY(float angle);
        void set_shearYX(float angle);
        
        vect3<float>* get_translation();
        vect3<float>* get_scale();
        vect3<float>* get_rotate();

        void set_rotate_mesh_x(float angle_x);
        void set_rotate_mesh_y(float angle_y);
        void set_rotate_mesh_z(float angle_z);

        Mat4x4 shearXY_matrix;
        Mat4x4 shearYX_matrix;

    private:        
        Display* display;
        vect3<float> cube_vertices[8];
        face cube_faces[12];
        uint32_t color = C_WHITE;
};

#endif