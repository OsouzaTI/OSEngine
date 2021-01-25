#ifndef OSENGINE_H
#define OSENGINE_H

#include "display.h"
#include "drawing.h"
#include "osgui.h"
#include "keyboard.h"
#include "vector.h"
#include "shapes.h"
#include "mesh.h"
#include "osmacros.h"
#include "osimgui.h"
#include "oscene_controller.h"

class OSEngine
{
	public:
		OSEngine();
		~OSEngine();
		bool game_loop;
		int position_mouse_x;
		int position_mouse_y;
		vect2<float> center_screen;

		SDL_Event keyboard_event;		
		Drawing draw;		
		Mesh mesh;		
		OSGuiController* GUI = OSGuiController::get_instance();
		OSImgui* OSIMGUI = OSImgui::get_instance();
		OSceneController* SCENE = OSceneController::get_instance();
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
		void run();

		int window_width();
		int window_height();
		Display* get_display();
		void create_camera(vect3<float> position, vect3<float> rotate, float fov, float znear, float zfar, float aspect = NULL);
		void set_camera_fov(float fov);
		// Metodos que ser�o sobrescritos pela classe filha
		virtual void engine_main() {}; // a cabe�a do jogo
		virtual void update() {}; // o pr� draw
		virtual void render() {}; // o draw		
		virtual void process_input() {}; // processamento do teclado e etc
		bool show_demo_window = true;
	private:
		Display display;
};

OSEngine::OSEngine(){

	game_loop = false;
	keyboard_event = SDL_Event();
	draw.set_display(&this->display);
	mesh.set_display(&this->display);
	position_mouse_x = 0;
	position_mouse_y = 0;
	OSGui::guidraw = &this->draw;
	OSGui::default_pallet = Pallets::sky_blue_rutvi;
	Shape::shapedraw = &this->draw;		
	SDL_StartTextInput();
}

OSEngine::~OSEngine(){
	SDL_StopTextInput();
	SDL_DestroyRenderer(this->display.get_renderer());
	SDL_DestroyWindow(this->display.get_window());
	TTF_Quit();
	SDL_Quit();
}

void OSEngine::create_window(const char* title, int width, int height)
{
	this->game_loop = this->display.init_window(title, width, height);	
	this->display.setup_window();	
	center_screen = { width / 2.0f, height / 2.0f };
	Shape::center_screen = &center_screen;
	// set the font
	OSGui::global_font = OSFont("F:\\Projects\\cpp\\OSEngine\\fonts\\OpenSans-Regular.ttf", 14);
	// imgui
	OSIMGUI->set_window(display.get_window());
	OSIMGUI->set_renderer(display.get_renderer());
	OSIMGUI->set_texture(display.get_texture());
	OSIMGUI->set_event(&keyboard_event);
	OSIMGUI->set_scene(SCENE->get_scene());
	OSIMGUI->set_scene_controller(SCENE);
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
	SCENE->draw();
	display.draw_buffer();
	OSIMGUI->draw();
	display.sdl_render_present();
}

void OSEngine::frame_rate_control()
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

void OSEngine::run()
{
	engine_main();
	OSIMGUI->setup(); // setup imgui
	while (this->game_loop) {		
		process_input();

		clear_screen();
		update();
		render();

		// update screen
		update_render();
	}
}

int OSEngine::window_width()
{
	return this->display.width;
}

int OSEngine::window_height()
{
	return this->display.height;
}

Display* OSEngine::get_display()
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
