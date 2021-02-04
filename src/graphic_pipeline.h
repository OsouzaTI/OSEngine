#ifndef GRAPHIC_PIPELINE_H
#define GRAPHIC_PIPELINE_H

#include "mesh.h"
#include "vector.h"
#include "texture.h"
#include "triangle.h"
#include <map>

class GraphicPipeline
{
	public:
		GraphicPipeline();
		~GraphicPipeline();

		void read_obj_file(std::string name, const char* filepath);
		void load_texture(const char* name, const char* filename);



		void process_image();	
		void process_image2();	

		void draw_pipeline();

		// getters and setter
		void set_display(Display* display);
		void set_drawing(Drawing* draw);

		Mesh* get_mesh(const char* name);

	private:
		Display* display;
		Drawing* draw;

		std::map<std::string, int> meshmap;
		std::vector<Mesh> meshs;

		//helpers
		uint32_t flat_shading(vect3<float> normal, uint32_t old_color);
		uint32_t smooth_shading(vect3<float> normal, uint32_t old_color);

		// functions do not use more
		void painters_algorithm();

};



#endif // !GRAPHIC_PIPELINE_H
