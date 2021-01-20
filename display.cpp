#include "display.h"
#include <iostream>

Display::Display()
{
	this->color_buffer = NULL;
	this->renderer = NULL;
	this->texture_buffer = NULL;
	this->window = NULL;
	this->title = "undefined";
	this->width = 0;
	this->height = 0;
	this->default_clear_color_buffer = 0xFF000000;
	this->previous_frame_time = 0;
	this->frame_target_time = 1000.0f/60;
	this->frame_rate = 0.0f;
	//this->buffer_title = NULL;
}

Display::~Display()
{
}

bool Display::init_window(const char* title, int width, int height)
{
	this->title = title;
	this->width = width;
	this->height = height;

	bool success = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Erro na inicialização do SDL\n";
		success = false;
	}

	if (TTF_Init() == -1) {
		std::cout << "Erro na inicialização do TTF_init\n";
		success = false;
	}

	this->window = SDL_CreateWindow(
		this->title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		this->width,
		this->height,
		SDL_WINDOW_MOUSE_CAPTURE
	);

	if (this->window == NULL) {
		std::cout << "Erro na criação da janela\n";
		success = false;
	}

	this->renderer = SDL_CreateRenderer(this->window, -1, 0);

	if (this->renderer == NULL) {
		std::cout << "Erro na criação do renderer\n";
		success = false;
	}

	return success;
}

void Display::setup_window()
{
	this->color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * this->width * this->height);
	this->texture_buffer = SDL_CreateTexture(
		this->renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		this->width,
		this->height
	);
}

void Display::draw_buffer()
{
	SDL_UpdateTexture(
		this->texture_buffer,
		NULL,
		this->color_buffer,
		static_cast<int>(this->width * sizeof(uint32_t))
	);
	SDL_RenderCopy(this->renderer, this->texture_buffer, NULL, NULL);
}

void Display::clear_buffer()
{
	for (int x = 0; x < this->width; x++)
	{
		for (int y = 0; y < this->height; y++)
		{
			this->color_buffer[pixel(x, y)] = this->default_clear_color_buffer;
		}
	}
}

SDL_Renderer* Display::get_renderer()
{
	return this->renderer;
}

void Display::create_camera(vect3<float> position, vect3<float> rotate, float fov, float znear, float zfar, float aspect)
{
	float _aspect = static_cast<float>(this->height / this->width);
	if (aspect != NULL) _aspect = aspect;
	this->camera = {
		position,
		rotate,
		fov,
		znear,
		zfar,
		_aspect
	};
}

void Display::set_camera_fov(float fov)
{
	this->camera.fov = fov;
}

void Display::set_clear_color_screen(uint32_t color)
{
	this->default_clear_color_buffer = color;
}

uint32_t* Display::get_color_buffer()
{
	return this->color_buffer;
}

void Display::frame_rate_control() {

	int time_to_wait = frame_target_time - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= frame_target_time)
		SDL_Delay(time_to_wait);

	this->frame_rate = 1000.0f / (SDL_GetTicks() - previous_frame_time);
	//this->frame_rate = (SDL_GetTicks() - previous_frame_time);

	previous_frame_time = SDL_GetTicks();
	sprintf(this->buffer_title, "%s | FPS: %.3f | FRT: %.3f ", this->title, this->frame_rate, this->frame_target_time);
	SDL_SetWindowTitle(this->window, this->buffer_title);

}

void Display::set_frame_rate(int frame_rate)
{
	this->frame_target_time = 1000.0f/frame_rate;
}
