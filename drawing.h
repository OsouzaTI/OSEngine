#ifndef DRAWING_H
#define DRAWING_H

#include "display.h"
#include "oscolor.hpp"

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
