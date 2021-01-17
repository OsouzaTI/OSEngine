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
		void update() override;
		void render() override;
	private:

};

Janela::Janela(){}
Janela::~Janela(){}

void Janela::engine_main() {
	create_window("Janela", 600, 600);
	mesh.load_obj_file_data("F:\\Projects\\cpp\\OSEngine\\obj_files\\f22.obj");
	mesh.set_display(get_display());

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
		update();
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

void Janela::update()
{
	mesh.update_mesh();
	frame_rate_control();
}

void Janela::render() {
	clear_screen();

	// Code
	mesh.draw_mesh(draw);
	update_render();
}

int main(int argv, char** args) {
	Janela game;
	game.engine_main();
	return 0;
}