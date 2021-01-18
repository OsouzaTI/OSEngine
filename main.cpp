#include <SDL2/SDL.h>
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

	// inicializando GUI
	dc1.GUI_init(&this->draw, 10, 10);
	dc2.GUI_init(&this->draw, 100, 100);
	sd.GUI_init(&this->draw, 10, 10, 300);

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
					mesh.set_rotate_mesh_z(RAD(angle_z));
					break;
				case OS_KEYBOARD_KEY::BT_ARROW_LEFT:
					mesh.set_rotate_mesh_x(RAD(angle_x));
					break;
				case OS_KEYBOARD_KEY::BT_ARROW_RIGHT:
					mesh.set_rotate_mesh_y(RAD(angle_y));
					break;
				case OS_KEYBOARD_KEY::BT_KEY_S:
					angle_x++;
					std::cout << "X: " << angle_x << std::endl;
					break;
				case OS_KEYBOARD_KEY::BT_KEY_D:
					angle_x--;
					std::cout << "X: " << angle_x << std::endl;
					break;
				case OS_KEYBOARD_KEY::BT_KEY_A:
					angle_y++;
					std::cout << "Y: " << angle_y << std::endl;
					break;
				case OS_KEYBOARD_KEY::BT_KEY_B:
					angle_y--;
					std::cout << "Y: " << angle_y << std::endl;
					break;
				case OS_KEYBOARD_KEY::BT_KEY_P:
					angle_z++;
					std::cout << "Z: " << angle_z << std::endl;
					break;
				case OS_KEYBOARD_KEY::BT_KEY_O:
					angle_z--;
					std::cout << "Z: " << angle_z << std::endl;
					break;
				default:
					break;
			}
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

	update_render();
}

int main(int argv, char** args) {
	Janela game;
	game.engine_main();
	return 0;
}