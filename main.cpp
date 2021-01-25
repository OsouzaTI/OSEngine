#include "os_engine.h"

class Janela : public OSEngine
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

	for (int i = 0; i < 3; i++) {
		char buffer[32];
		sprintf(buffer, "Elipse %d", i);
		SCENE->add<Ellipse>(
			buffer,
			vect3<float>{ 1.0f, 1.0f, 1.0f },
			vect3<float>{ 1.0f, 1.0f, 1.0f },
			vect3<float>{ 0.0f, 0.0f, 0.0f }, 100.0f);
	}


	create_camera(
		{0, 0, 0},
		{0, 0 ,0},
		60.0f,
		0.1f,
		100.0f		
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
}

void Janela::render() {	
	


}

int main(int argv, char** args) {	
	Janela game;
	game.run();
	return 0;
}