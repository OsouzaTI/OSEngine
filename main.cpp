#include <SDL.h>
#include <iostream>

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

		DraggableCircle dc1;
		DraggableCircle dc2;
		SliderRange sd;
		TextLabel lb1;

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
}

void Janela::render() {
	clear_screen();
	
	sd.GUI_draw();
	

	draw.draw_circle(this->window_width()/2, this->window_height()/2, sd.get_value(), C_RED);

	draw_buffer();
	// O TextLabel deve ser chamado apos os GUI que usam o draw buffer comumente
	// ele utiliza textura diretamente copiadas acima do renderer
	lb1.GUI_draw();

	// por ultimo sempre se deve chamar o update para que apareça as
	// coisa em sua tela
	update_render();
}

int main(int argv, char** args) {
	Janela game;
	game.engine_main();
	return 0;
}