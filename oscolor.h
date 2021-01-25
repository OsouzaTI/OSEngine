#ifndef OSCOLOR_H
#define OSCOLOR_H

#include <SDL.h>

static SDL_Color CS_WHITE = { 255, 255, 255 };
static SDL_Color CS_RED = { 255, 0, 0 };
static SDL_Color CS_GREEN = { 0, 255, 0 };

enum COLORS {
	C_WHITE = 0xFFFFFFFF, // white color hex
	C_BLACK = 0xFF000000, // black color hex
	C_RED = 0xFFFF0000, // red color hex
	C_GREEN = 0xFF00FF00, // green color hex
	C_BLUE = 0xFF0000FF, // blue color hex
	C_BLUE_CLEAN = 0x9ac5db, // blue color hex
	C_YELLOW = 0xFFFFFF00, // yellow color hex
	C_PINK_CLEAN = 0xFFF00640, // yellow color hex
	C_PINK = 0xd61195, // pink color hex
	C_IMAGINARY_AXIOS = 0xFFFAB18E, // white 100-alpha,
	C_MIDNIGHT = 0xFF5b1865,
	C_TYRIAN_PURPLE = 0xFF470024,
	C_TYRIAN_BLUE = 0xFF390099,
	C_JAZZBERRY_JAM = 0xFF9e0059,
	C_MEXICAN_PINK = 0xFFe0007f,

	// Pallet 1 : Blue
	C_PALLET_1_BLUE = 0xFFa49fc7,
	C_PALLET_2_BLUE = 0xFF8280b8,
	C_PALLET_3_BLUE = 0xFF6667a9,
	C_PALLET_4_BLUE = 0xFF464c9c,
	C_PALLET_5_BLUE = 0xFF18197e,
};

typedef struct 
{
	uint32_t color1;
	uint32_t color2;
	uint32_t color3;
	uint32_t color4;
	uint32_t color5;
	SDL_Color text_color = CS_WHITE;
} OSPallet;

namespace Pallets {

	static OSPallet blue_rutvi{
		0xFFa49fc7,
		0xFF8280b8,
		0xFF6667a9,
		0xFF464c9c,
		0xFF18197e,
		CS_GREEN
	};

	static OSPallet orange_rutvi{
		0xFFffbb8d,
		0xFFffab7e,
		0xFFffa05f,
		0xFFfb8e33,
		0xFFff7400		
	};

	static OSPallet sky_blue_rutvi{
		0xFF9edbdd,
		0xFF8dcdd2,
		0xFF55babd,
		0xFF3b9896,
		0xFF2f6567
	};

}

static uint32_t rgb_to_hex(int r, int g, int b) {
	r = r > 255 ? 255 : r;
	g = g > 255 ? 255 : g;
	b = b > 255 ? 255 : b;	
	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

static vect3<float> hex_to_rgb(uint32_t hex_color) {
	vect3<float> color;
	color.x = ((hex_color >> 16) & 0xFF) / 255;  // Extract the RR byte
	color.y = ((hex_color >> 8) & 0xFF) / 255;   // Extract the GG byte
	color.z = ((hex_color) & 0xFF) / 255;        // Extract the BB byte
	return color;
}

static uint32_t color32_reverse(uint32_t x)
{
	return
		// Source is in format: 0xAARRGGBB
		((x & 0xFF000000) >> 24) | //______AA
		((x & 0x00FF0000) >> 8) | //____RR__
		((x & 0x0000FF00) << 8) | //__GG____
		((x & 0x000000FF) << 24);  //BB______
}

#endif // !OSCOLOR_H
