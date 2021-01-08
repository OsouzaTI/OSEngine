#ifndef DRAWING_H
#define DRAWING_H

#include "display.h"

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
	C_IMAGINARY_AXIOS = 0xFFFAB18E, // white 100-alpha
};

class Drawing
{
	public:
		Drawing();
		~Drawing();

		int pixel(int x, int y);
		void set_display(Display* display);
		void draw_pixel(int x, int y, uint32_t color);
		void draw_rect(int x, int y, int w, int h, int border_size, uint32_t color);
		void draw_fill_rect(int x, int y, int w, int h, uint32_t color);
		void draw_circle(int x, int y, int radius, uint32_t color);
		void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
		void draw_line_border(int x0, int y0, int x1, int y1, int border, uint32_t color);
		void draw_grid(int grid_size, uint32_t color);
	private:
		Display* display;
};

#endif // !DRAWING_H
