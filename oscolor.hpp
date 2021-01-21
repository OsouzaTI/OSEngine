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
} OSPallet;

static OSPallet blue_pallet{
	0xFFa49fc7,
	0xFF8280b8,
	0xFF6667a9,
	0xFF464c9c,
	0xFF18197e
};

#endif // !OSCOLOR_H
