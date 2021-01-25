#include "os_engine.hpp"

class Example : public OSEngine
{
public:
	Example() {};
	~Example() {};

	void engine_main() override;
	void process_input() override;
	void update() override;
	void render() override;

private:

};

void Example::engine_main()
{
	create_window("Example window", 400, 400);
	while (game_loop) {
		process_input();
		update();
		render();
	}
}

void Example::process_input()
{
	read_event();
	switch (keyboard_event_type()) {
	case OS_KEYBOARD_TYPE::QUIT: game_loop = false;
	default: break;
	}
}

void Example::update()
{
	frame_rate_control();
}

void Example::render() {
	clear_screen();
	// code
	update_render();
}

int main(int argc, char* argv[]) {
	Example example;
	example.engine_main();
	return 0;
}