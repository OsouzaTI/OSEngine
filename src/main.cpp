#include "renderer.h"

class Janela : public OSRenderer
{
	public:
		Janela() {};
		~Janela() {};
		
		TransformInput* escala;
		TransformInput* rotacao;		
		TextLabel* num_triangles;

		void engine_main() override;
		void process_input() override;
		void update() override;
		void render() override;			
	private:

};

void Janela::engine_main() {
	create_window("Janela", 800, 800, IMGUI);		
	create_camera(
		{0, 0, 0},
		{0, 0 ,1},
		0.0f,
		100.0f,		
		1.0f,
		200.0f
	);

	num_triangles = &GUI->add<TextLabel>("Teste",10, 10);
	num_triangles->set_text_color({ 255, 0, 0 });
	
	pipeline.read_obj_file("F:\\Projects\\cpp\\OSRenderer\\Objects3D\\obj\\_cube.obj");
	pipeline.load_texture("F:\\Projects\\cpp\\OSRenderer\\Objects3D\\textures\\cube.png");

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
				case OS_KEYBOARD_KEY::BT_KEY_W: {
					Display::camera->forward_velocity = smultvect<float>(Display::camera->direction, 1.0f * delta_time());
					Display::camera->position = vsumvect(
						Display::camera->position,
						Display::camera->forward_velocity
					);
				}break;
				case OS_KEYBOARD_KEY::BT_KEY_S: {
					Display::camera->forward_velocity = smultvect<float>(Display::camera->direction, 1.0f * delta_time());
					Display::camera->position = vsubvect(
						Display::camera->position,
						Display::camera->forward_velocity
					);
				}break;
				case OS_KEYBOARD_KEY::BT_KEY_A:
					Display::camera->yaw += 1.0f * delta_time();
					break;
				case OS_KEYBOARD_KEY::BT_KEY_D:
					Display::camera->yaw -= 3.0f * delta_time();
					break;
				case OS_KEYBOARD_KEY::BT_ARROW_UP:
					Display::camera->position.y += 3.0f * delta_time();
					break;
				case OS_KEYBOARD_KEY::BT_ARROW_DOWN:
					Display::camera->position.y -= 3.0f * delta_time();
					break;
			}		
		default:
			break;
	}

}

void Janela::update()
{
	frame_rate_control();	
	SDL_GetMouseState(&position_mouse_x, &position_mouse_y);	
	char buffer[100];
	sprintf(buffer, "Numero de triangulos: %d", pipeline.get_mesh()->number_of_triangles_to_render);
	num_triangles->set_text(buffer);	
	pipeline.process_image2();

}

void Janela::render() {	

	pipeline.draw_pipeline();

}

int main(int argv, char** args) {	
	Janela game;
	game.run();
	return 0;
}