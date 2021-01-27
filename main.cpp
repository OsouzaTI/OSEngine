#include "osrenderer.h"

class Janela : public OSRenderer
{
	public:
		Janela() {};
		~Janela() {};
		
		TransformInput* escala;
		TransformInput* rotacao;		

		void engine_main() override;
		void process_input() override;
		void update() override;
		void render() override;			
	private:

};

void Janela::engine_main() {
	create_window("Janela", 1200, 800);		
	GUI->add<TransformInput>("Teste",10, 10);	
	mesh.load_cube_object();
	create_camera(
		{0, 0, 0},
		{0, 0 ,0},
		60.0f,
		0.1f,
		100.0f,
		(float)window_height()/window_width()
	);

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
			switch (keyboard_event_key()){			
			}
		default:
			break;
	}

}

void Janela::update()
{
	frame_rate_control();	
	SDL_GetMouseState(&position_mouse_x, &position_mouse_y);	
	mesh.update_mesh();
}

void Janela::render() {	
	mesh.draw_mesh();
}

int main(int argv, char** args) {	
	Janela game;
	game.run();
	return 0;
}