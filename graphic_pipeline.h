#ifndef GRAPHIC_PIPELINE_H
#define GRAPHIC_PIPELINE_H

#include "mesh.h"
#include "vector.h"
#include "texture.h"
#include "triangle.h"

class GraphicPipeline
{
	public:
		GraphicPipeline();
		~GraphicPipeline();

		void read_obj_file(const char* filename);
		void load_texture(const char* filename);

		void object_space();
		void world_space();

		void process_image();
		void draw_pipeline();
		void reset_matrix();

		// getters and setter
		void set_display(Display* display);
		void set_drawing(Drawing* draw);

		Mesh* get_mesh();

	private:
		Display* display;
		Drawing* draw;

		Mesh _mesh;

		//helpers
		uint32_t flat_shading(vect3<float> normal, uint32_t old_color);

		// functions do not use more
		void painters_algorithm();

};



#endif // !GRAPHIC_PIPELINE_H
