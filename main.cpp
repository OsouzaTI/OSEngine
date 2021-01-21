#include "os_engine.hpp"

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

		AreaLogger logger;		
		Ellipse elipse;		
		TextInput input;
		OSFont font;


		void engine_main() override;
		void process_input() override;
		void update() override;
		void render() override;			
	private:

};

Janela::Janela(){
	this->elipse = Ellipse(
		{ 100, 100, 0 },
		{ 1, 1, 1 },
		{ 0, 0, 0 },
		50.0f
	);
}

Janela::~Janela(){}

void Janela::engine_main() {
	create_window("Janela", 800, 600);
	this->font.open_font("F:\\Projects\\cpp\\OSEngine\\fonts\\aAbsoluteEmpire.ttf", 12);
	//this->mesh.load_obj_file_data("F:\\Projects\\cpp\\OSEngine\\obj_files\\MONARCH.OBJ");
	// inicializando GUI
	logger.GUI_init(0, -(250-window_height()), window_width(), 250, this->font);
	logger.lock_view_end(true);
	input.GUI_init("Nome: ", 10, 10, font);

	create_camera(
		{0, 0, 0},
		{0, 0 ,0},
		60.0f,
		0.1f,
		100.0f		
	);

	if (this->game_loop)
		std::cout << "OPA\n";
	
	SDL_StartTextInput();
	while (this->game_loop) {
		process_input();
		update();
		render();				
	}
	SDL_StopTextInput();
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
	SDL_GetMouseState(&position_mouse_x, &position_mouse_y);	
	logger.add_log(STRLOG("Mouse X: ")+NSTR(position_mouse_x)+STR(" Mouse Y: ")+NSTR(position_mouse_y));
}

void Janela::render() {
	clear_screen();	
	
	elipse.draw();
	logger.draw();
	input.draw();

	update_render();
}

int main(int argv, char** args) {
	Janela game;
	game.engine_main();
	return 0;
}