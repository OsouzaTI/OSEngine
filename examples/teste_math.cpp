#include "osrenderer.h"

class Example : public OSRenderer
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
	create_window("Math teste", 200, 200, NONGUI);
	create_camera(
		{ 0, 0, 0 },
		{ 0, 0 ,0 },
		60.0f,
		0.1f,
		100.0f,
		(float)window_height() / window_width()
	);

	float n1 = 0.45f;
	std::cout << Math::clamp<float>(n1, 0.1f, 0.5f) << std::endl;


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
	// code
}

int main(int argc, char* argv[]) {
	Example example;
	example.run();
	return 0;
}