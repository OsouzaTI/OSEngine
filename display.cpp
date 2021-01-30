#include "display.h"
#include <iostream>
#include <assert.h>

Camera* Display::camera = NULL;

Display::Display()
{
	color_buffer = NULL;
	renderer = NULL;
	texture_buffer = NULL;
	window = NULL;
	title = "undefined";
	width = 0;
	height = 0;
	default_clear_color_buffer = 0xFF000000;
	previous_frame_time = 0;
	frame_target_time = 1000.0f/60;
	frame_rate = 0.0f;
	buffer_title = new char[200];	

	frustum = FrustumPlane::get_instance();

}

Display::~Display()
{
	free(color_buffer);
	free(z_buffer);
}

bool Display::init_window(const char* title, int width, int height, int wview_port, int hview_port, GUI_MODE mode)
{
	this->title = title;
	this->width = width;
	this->height = height;
	view_port = { wview_port, hview_port };
	gui_mode = mode;
	bool success = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Erro na inicialização do SDL\n";
		success = false;
	}

	if (TTF_Init() == -1) {
		std::cout << "Erro na inicialização do TTF_init\n";
		success = false;
	}
	else {
		std::cout << "TTF inicializado\n";
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
	this->color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * view_port.width * view_port.height);
	this->z_buffer	   = (float*)malloc(sizeof(float) * view_port.width * view_port.height);
	if (color_buffer == NULL && z_buffer != NULL) {
		std::cout << "Error in memory" << std::endl;		
	}

	this->texture_buffer = SDL_CreateTexture(
		this->renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_TARGET,
		view_port.width,
		view_port.height
	);
}

void Display::draw_buffer()
{
	SDL_UpdateTexture(
		this->texture_buffer,
		NULL,
		this->color_buffer,
		static_cast<int>(view_port.width * sizeof(uint32_t))
	);
	SDL_RenderCopy(this->renderer, this->texture_buffer, NULL, NULL);
}

void Display::clear_buffer()
{
	for (int x = 0; x < view_port.width; x++)
	{
		for (int y = 0; y < view_port.height; y++)
		{
			this->color_buffer[pixel(x, y)] = this->default_clear_color_buffer;
		}
	}
}

void Display::clear_z_buffer()
{
	for (int x = 0; x < view_port.width; x++)
	{
		for (int y = 0; y < view_port.height; y++)
		{
			this->z_buffer[pixel(x, y)] = 1.0f;
		}
	}
}

SDL_Renderer* Display::get_renderer()
{
	return this->renderer;
}

SDL_Window* Display::get_window()
{
	return this->window;
}

SDL_Texture* Display::get_texture()
{
	return texture_buffer;
}

Camera* Display::get_camera()
{
	return Display::camera;
}

void Display::create_camera(vect3<float> position, vect3<float> rotate, float fov, float znear, float zfar, float aspect)
{
	//float _aspect = view_port.height / (float)view_port.width;
	//if (aspect != NULL) _aspect = aspect;
	//this->camera = {
	//	position,
	//	rotate,
	//	fov,
	//	znear,
	//	zfar,
	//	_aspect
	//};
}

void Display::set_camera_fov(float fov)
{
	Display::camera->fov = fov;
}

void Display::set_clear_color_screen(uint32_t color)
{
	this->default_clear_color_buffer = color;
}

uint32_t* Display::get_color_buffer()
{
	return this->color_buffer;
}

float* Display::get_z_buffer()
{
	return z_buffer;
}

void Display::frame_rate_control() {

	int time_to_wait = frame_target_time - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= frame_target_time)
		SDL_Delay(time_to_wait);

	this->frame_rate = 1000.0f / (SDL_GetTicks() - previous_frame_time);
	//this->frame_rate = (SDL_GetTicks() - previous_frame_time);

	delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0f;

	previous_frame_time = SDL_GetTicks();
	sprintf(this->buffer_title, "%s | FPS: %.3f | FRT: %.3f ", this->title, this->frame_rate, this->frame_target_time);
	SDL_SetWindowTitle(this->window, this->buffer_title);

}

void Display::set_frame_rate(int frame_rate)
{
	this->frame_target_time = 1000.0f/frame_rate;
}
