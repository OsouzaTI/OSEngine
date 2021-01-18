#ifndef DRAWING_H
#define DRAWING_H

#include "display.h"

static SDL_Color CS_WHITE = { 255, 255, 255 };
static SDL_Color CS_RED   = { 255, 0, 0 };

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
};

typedef struct 
{
	int a;
	int b;
	int c;
	int s;
	int ymax;
	int ymin;
} ImplicitLine;

class Drawing
{
	public:
		Drawing();
		~Drawing();

		Display* get_display();
		void set_display(Display* display);

		int pixel(int x, int y);
		void draw_pixel(int x, int y, uint32_t color);
		void draw_rect(int x, int y, int w, int h, int border_size, uint32_t color);
		void draw_fill_rect(int x, int y, int w, int h, uint32_t color);
		void draw_circle(int x, int y, int radius, uint32_t color);
		void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
		void draw_line_border(int x0, int y0, int x1, int y1, int border, uint32_t color);
		void draw_grid(int grid_size, uint32_t color);

		// Primitive
		void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
		void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
		void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
		void draw_fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

		// Implicit lines
		ImplicitLine implicit_line(int x1, int y1, int x2, int y2);
		int implicit_line_winding_number(ImplicitLine implicit_line, int x, int y);
		void draw_implicit_line(int x1, int y1, int x2, int y2, uint32_t color);
	private:
		Display* display;
};

#endif // !DRAWING_H
