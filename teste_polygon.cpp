#include "os_engine.hpp"

class Polygon : public OSEngine
{
public:
	Polygon() {};
	~Polygon() {};

	void hline(int y, int x1, int x2);
	void edge(int x1, int y1, int x2, int y2);

	void engine_main() override;
	void process_input() override;
	void update() override;
	void render() override;

private:

};

void Polygon::engine_main()
{
	create_window("Polygon", 400, 400);
	while (game_loop) {
		process_input();
		update();
		render();
	}
}

void Polygon::process_input()
{
	read_event();
	switch (keyboard_event_type()) {
		case OS_KEYBOARD_TYPE::QUIT: game_loop = false;
		default: break;
	}
}

void Polygon::update()
{
	frame_rate_control();
}
float ang = 0.0f;
void Polygon::render()
{
	clear_screen();
	vect2<float> center = { window_width() / 2.0f, window_height() / 2.0f };
	float scale = 50.0f;

	
	ang += 0.01f;
	update_render();
}

int main(int argc, char* argv[]){
	Polygon polygon;
	polygon.engine_main();
	return 0;
}