#include <SDL.h>
#include <iostream>
#include <string>
#include "os_engine.hpp"
#include "osmath.hpp"

class Janela : public OSEngine
{
	public:
		Janela();
		~Janela();
		float fov = 60.0f;	
		int angle_x = 0;
		int angle_y = 0;
		int angle_z = 0;
		int teste_counter = 0;
		DraggableCircle dc1;
		DraggableCircle dc2;
		SliderRange sd;
		TextLabel lb1;
		AreaLogger logger;

		void engine_main() override;
		void process_input() override;
		void update() override;
		void render() override;			
	private:

};

Janela::Janela(){}
Janela::~Janela(){}

void Janela::engine_main() {
	create_window("Janela", 800, 600);

	// inicializando GUI
	dc1.GUI_init(&this->draw, 10, 10);
	dc2.GUI_init(&this->draw, 100, 100);
	sd.GUI_init(&this->draw, 10, 10, 300);
	lb1.GUI_init(&this->draw, "Ozeias", 10, 200, 16);
	logger.GUI_init(&this->draw, 0, -(250-window_height()), window_width(), 250);
	logger.set_logger_view(true);
	for (int i = 0; i < 100; i++)
	{
		std::string texto = std::string("Numero ") + std::to_string(i);
		logger.add_log(texto.c_str());
	}

	create_camera(
		{0, 0, 0},
		{0, 0 ,0},
		60.0f,
		0.1f,
		100.0f
	);

	if (this->game_loop)
		std::cout << "OPA\n";

	while (this->game_loop) {
		process_input();
		update();
		render();				
	}

}

void Janela::process_input()
{

	read_event();	
	switch (keyboard_event_type())
	{
		case OS_KEYBOARD_TYPE::QUIT:
			this->game_loop = false;
			break;
		case OS_KEYBOARD_TYPE::KEY_DOWN:
			switch (keyboard_event_key())
			{}
		default:
			break;
	}

}

void Janela::update()
{
	frame_rate_control();
	if (teste_counter++ > 500) {
		teste_counter = 0;
	}		
	
	SDL_GetMouseState(&position_mouse_x, &position_mouse_y);	
	logger.add_log(STRLOG("Mouse X: ")+NSTR(position_mouse_x)+STR(" Mouse Y: ")+NSTR(position_mouse_y));
	
}

void Janela::render() {
	clear_screen();	
	logger.GUI_draw();
	update_render();
}

int main(int argv, char** args) {
	Janela game;
	game.engine_main();
	return 0;
}