#include "texture.h"

OSTexture::OSTexture()
{
	png_texture = NULL;
	mesh_texture = NULL;
}

OSTexture::~OSTexture()
{
}

void OSTexture::load_png_texture(const char* filename)
{
	png_texture = upng_new_from_file(filename);
	if (png_texture != NULL) {
		upng_decode(png_texture);
		if (upng_get_error(png_texture) == UPNG_EOK) {
			mesh_texture = (uint32_t*)upng_get_buffer(png_texture);
			texture_width = upng_get_width(png_texture);
			texture_height = upng_get_height(png_texture);
			std::cout << texture_height << std::endl;
		}
		else {
			std::cout << "erro in load png texture" << std::endl;
		}
	}
	else {
		std::cout << "erro in open file texture" << std::endl;
	}
}