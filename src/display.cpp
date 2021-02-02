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
		LOG("inicialization error of SDL");
		success = false;
	}else{ LOG("SDL started"); }

	if (TTF_Init() == -1) {
		LOG("inicialization error of SDL tff");
		success = false;
	}
	else { LOG("SDL_ttf started"); }
	
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		LOGERR("inicialization error of SDL image", IMG_GetError());
		success = false;
	}else{ LOG("SDL_image started"); }

	window = SDL_CreateWindow(
		this->title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		this->width,
		this->height,
		SDL_WINDOW_MOUSE_CAPTURE		
	);

	if (this->window == NULL) {
		LOG("inicialization error in window");
		success = false;
	}
	else { LOG("window started"); }

	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

	if (this->renderer == NULL) {
		LOG("inicialization error in renderer");
		success = false;
	}
	else { LOG("renderer started"); }

	if (success) {
		LOG("all the operations result in success");
	}

	return success;
}

void Display::setup_window()
{
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * view_port.width * view_port.height);
	z_buffer	   = (float*)malloc(sizeof(float) * view_port.width * view_port.height);

	if (color_buffer == NULL && z_buffer != NULL) {
		LOG("error in alocated dinamically memory for color_buffer or z_buffer");		
	}else{ LOG("Buffer color/z allocated"); }

	texture_buffer = SDL_CreateTexture(
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

void Display::create_camera(
	vect3<float> position,
	vect3<float> direction,
	float yaw,
	float fov_x,
	float fov_y,
	float znear,
	float zfar,
	float aspect_x,
	float aspect_y
)
{
	Display::camera = new Camera(
		position,
		direction,
		yaw,
		fov_x,
		fov_y,
		znear,
		zfar,
		aspect_x,
		aspect_y
	);
}

void Display::set_camera_fov(float fov_x, float fov_y)
{
	Display::camera->fov_x = fov_x;
	Display::camera->fov_y = fov_y;
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
