#ifndef OSENGINE_H
#define OSENGINE_H
#include <iostream>
#include "display.h"
#include "drawing.h"
#include "osgui.hpp"
#include "keyboard.h"
#include "vector.hpp"
#include "shapes.hpp"
#include "mesh.h"
#include <string>
#include <string.h>
#include "osmacros.hpp"

class OSEngine
{
	public:
		OSEngine();
		~OSEngine();
		bool game_loop;
		int position_mouse_x;
		int position_mouse_y;
		SDL_Event keyboard_event;		
		Drawing draw;		
		Mesh mesh;
		// Cria a janela
		void create_window(const char* title, int width, int height);
		// Limpa o buffer de cor
		void clear_screen(); // this->display.clear_buffer();
		// desenha o buffer
		void draw_buffer();
		// Atualiza o render
		void update_render();
		
		void frame_rate_control();
		// Ler os eventos do teclado
		inline void read_event() { SDL_PollEvent(&this->keyboard_event); };
		// retorna o tipo de evento que foi disparado
		inline Uint32 keyboard_event_type() { return this->keyboard_event.type; };
		inline Uint32 keyboard_event_key() { return this->keyboard_event.key.keysym.sym; };

		void set_frame_rate(int frame_rate);
		void set_background_color(uint32_t color);

		int window_width();
		int window_height();
		Display* get_display();
		void create_camera(vect3<float> position, vect3<float> rotate, float fov, float znear, float zfar, float aspect = NULL);
		void set_camera_fov(float fov);
		// Metodos que ser�o sobrescritos pela classe filha
		virtual void engine_main() {}; // a cabe�a do jogo
		virtual void update() {}; // o pr� draw
		virtual void render() {}; // o draw
		// virtual void GUI_render() {}; // render GUI -- ??
		virtual void process_input() {}; // processamento do teclado e etc

	private:
		Display display;
};

OSEngine::OSEngine(){
	this->game_loop = false;
	this->keyboard_event = SDL_Event();
	this->draw.set_display(&this->display);
	OSGui::guidraw = &this->draw;
	Shape::shapedraw = &this->draw;
}

OSEngine::~OSEngine(){}

void OSEngine::create_window(const char* title, int width, int height)
{
	this->game_loop = this->display.init_window(title, width, height);	
	this->display.setup_window();
}

void OSEngine::clear_screen()
{
	this->display.clear_buffer();
}

void OSEngine::draw_buffer()
{
	this->display.draw_buffer();
}

void OSEngine::update_render()
{	
	this->display.sdl_render_present();
	this->display.draw_buffer();
}

inline void OSEngine::frame_rate_control()
{
	this->display.frame_rate_control();
}

void OSEngine::set_frame_rate(int frame_rate)
{
	this->display.set_frame_rate(frame_rate);
}

void OSEngine::set_background_color(uint32_t color)
{
	this->display.set_clear_color_screen(color);
}

int OSEngine::window_width()
{
	return this->display.width;
}

inline int OSEngine::window_height()
{
	return this->display.height;
}

inline Display* OSEngine::get_display()
{
	return &this->display;
}

void OSEngine::create_camera(vect3<float> position, vect3<float> rotate, float fov, float znear, float zfar, float aspect)
{
	if (aspect != NULL)
		this->display.create_camera(position, rotate, fov, znear, zfar, aspect);
	else
		this->display.create_camera(position, rotate, fov, znear, zfar);
}

void OSEngine::set_camera_fov(float fov)
{
	this->display.set_camera_fov(fov);
}

#endif // !OSENGINE_H
