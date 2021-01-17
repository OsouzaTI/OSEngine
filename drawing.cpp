#include "drawing.h"
#include "osmath.hpp"
#include <iostream>

Drawing::Drawing() {
	this->display = NULL;
}
Drawing::~Drawing() {}

int Drawing::pixel(int x, int y)
{
	return ((this->display->width * y) + x);
}

void Drawing::set_display(Display* display)
{
	this->display = display;
}

void Drawing::draw_pixel(int x, int y, uint32_t color)
{
    if (x > this->display->width || x < 0 || y > this->display->height || y < 0)
        return;

    uint32_t* color_buffer = this->display->get_color_buffer();
    color_buffer[pixel(x, y)] = color;
}

void Drawing::draw_rect(int x, int y, int w, int h, int border_size, uint32_t color) {

    for (int yy = 0; yy < h; yy++)
    {
        for (int xx = 0; xx < w; xx++)
        {
            int _xx = x + xx;
            int _yy = y + yy;
            if (xx == 0 || xx <= 0 + border_size)
                draw_pixel(_xx, _yy, color);
            if (yy == 0 || yy <= 0 + border_size)
                draw_pixel(_xx, _yy, color);
            if (yy == h - 1 || yy >= h - border_size)
                draw_pixel(_xx, _yy, color);
            if (xx == w - 1 || xx >= w - border_size)
                draw_pixel(_xx, _yy, color);

        }
    }

}

void Drawing::draw_fill_rect(int x, int y, int w, int h, uint32_t color) {

    for (int yy = 0; yy < h; yy++)
    {
        for (int xx = 0; xx < w; xx++)
        {
            int _xx = x + xx;
            int _yy = y + yy;
            draw_pixel(_xx, _yy, color);
        }
    }

}

void Drawing::draw_circle(int x, int y, int radius, uint32_t color) {

    int xx = 0;
    int yy = radius;
    int dd = 3 - (2 * radius);

    draw_pixel(x + xx, y + yy, color);
    draw_pixel(x - xx, y + yy, color);
    draw_pixel(x + xx, y - yy, color);
    draw_pixel(x - xx, y - yy, color);

    draw_pixel(x + yy, y + xx, color);
    draw_pixel(x - yy, y + xx, color);
    draw_pixel(x + yy, y - xx, color);
    draw_pixel(x - yy, y - xx, color);

    while (yy >= xx)
    {
        xx++;
        if (dd > 0)
        {
            yy--;
            dd = dd + 4 * (xx - yy) + 10;
        }
        else {
            dd = dd + 4 * xx + 6;
        }

        draw_pixel(x + xx, y + yy, color);
        draw_pixel(x - xx, y + yy, color);
        draw_pixel(x + xx, y - yy, color);
        draw_pixel(x - xx, y - yy, color);
        draw_pixel(x + yy, y + xx, color);
        draw_pixel(x - yy, y + xx, color);
        draw_pixel(x + yy, y - xx, color);
        draw_pixel(x - yy, y - xx, color);
    }


}

// Draw line algorithm DDA
// comments in draw line are portuguese
void Drawing::draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    // calcula os deltas respectivos
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);
    // variavel destinada ao maior delta
    int side_lenght = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);
    // taxa de incremento dos eixos
    float x_inc = delta_x / (float)side_lenght;
    float y_inc = delta_y / (float)side_lenght;

    // valor atual do incrementos
    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= side_lenght; i++)
    {
        draw_pixel((int)round(current_x), (int)round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }

}

void Drawing::draw_line_border(int x0, int y0, int x1, int y1, int border_size, uint32_t color) {
    // para saber o que o algoritmo esta fazendo
    // verifique a função draw_line

    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int side_lenght = 1;
    if (abs(delta_x) >= abs(delta_y)) {
        side_lenght = abs(delta_x);
    }
    else {
        side_lenght = abs(delta_y);
    }

    float x_inc = delta_x / (float)side_lenght;
    float y_inc = delta_y / (float)side_lenght;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i < side_lenght; i++)
    {
        draw_fill_rect(
            (int)current_x,
            (int)current_y,
            border_size,
            border_size,
            color
        );
        current_x += x_inc;
        current_y += y_inc;
    }


}

void Drawing::draw_grid(int grid_size, uint32_t color) {
    for (int i = 0; i < this->display->width; i += grid_size)
    {
        for (int j = 0; j < this->display->height; j += grid_size)
        {
            draw_pixel(i, j, color);
        }
    }
}

void Drawing::fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float inv_slope_1 = (float)(x1 - x0) / (y1 - y0);
    float inv_slope_2 = (float)(x2 - x0) / (y2 - y0);

    float x_start = x0;
    float x_end = x0;

    for (int y = y0; y <= y2; y++)
    {
        draw_line((int)x_start, y, (int)x_end, y, color);
        x_start += inv_slope_1;
        x_end += inv_slope_2;
    }
}

void Drawing::fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float inv_slope_1 = (float)(x2 - x0) / (y2 - y0);
    float inv_slope_2 = (float)(x2 - x1) / (y2 - y1);

    float x_start = x2;
    float x_end = x2;

    for (int y = y2; y >= y0; y--)
    {
        draw_line((int)x_start, y, (int)x_end, y, color);
        x_start -= inv_slope_1;
        x_end -= inv_slope_2;
    }
}

void Drawing::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void Drawing::draw_fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    if (y0 > y1) {
        Math::swap(&y0, &y1);
        Math::swap(&x0, &x1);
    }
    if (y1 > y2) {
        Math::swap(&y1, &y2);
        Math::swap(&x1, &x2);
    }
    if (y0 > y1) {
        Math::swap(&y0, &y1);
        Math::swap(&x0, &x1);
    }


    if (y1 == y2) {
        // Draw flat-bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else if (y0 == y1) {
        // Draw flat-top triangle
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else {

        //vec2_t mid = GeoMath::triangle_mid_point(t);
        int My = y1;
        int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;
        // Draw flat-bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);
        // Draw flat-top triangle
        fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);

    }
}
