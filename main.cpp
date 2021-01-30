#include "renderer.h"

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
	create_window("Janela", 1000, 800, IMGUI);		
	GUI->add<TransformInput>("Teste",10, 10);	
	//mesh.mesh.translation.y = -900;
	//mesh.mesh.translation.z = 0;
	mesh.load_obj_file_dataV2(
		"F:\\Projects\\cpp\\OSRenderer\\Objects3D\\obj\\f22.obj");
	mesh.texture.load_png_texture(
		"F:\\Projects\\cpp\\OSRenderer\\Objects3D\\textures\\f22.png");
	//mesh.load_cube_object();
	create_camera(
		{0, 0, 0},
		{0, 0 ,1},
		0.0f,
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
				case OS_KEYBOARD_KEY::BT_KEY_W: {
					Display::camera->forward_velocity = smultvect<float>(Display::camera->direction, 5.0f * delta_time());
					Display::camera->position = vsumvect(
						Display::camera->position,
						Display::camera->forward_velocity
					);
				}break;
				case OS_KEYBOARD_KEY::BT_KEY_S: {
					Display::camera->forward_velocity = smultvect<float>(Display::camera->direction, 5.0f * delta_time());
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