#include <SDL2/SDL.h>
#include <iostream>

#include "os_engine.hpp"

class Janela : public OSEngine
{
	public:
		Janela();
		~Janela();
		void engine_main() override;
		void process_input() override;
		//void update() override;
		void render() override;
	private:

};

Janela::Janela(){}
Janela::~Janela(){}

void Janela::engine_main() {
	create_window("Janela", 400, 300);
	if (this->game_loop)
		std::cout << "OPA\n";

	while (this->game_loop) {
		render();
		process_input();
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
			{
				case OS_KEYBOARD_KEY::BT_ARROW_UP:
					std::cout << "UP\n";
				default:
					break;
			}
		default:
			break;
	}

}

void Janela::render() {
	clear_screen();

	// Code
	draw.draw_rect(10, 10, 100, 30, 1, 0xFFFFFF00);

	update_render();
}

int main(int argv, char** args) {
	Janela game;
	game.engine_main();
	return 0;
}