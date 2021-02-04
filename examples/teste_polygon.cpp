#include "renderer.h"

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
	create_window("Example window", 400, 400, NONGUI);
	// default camera
	create_camera(
		{ 0, 0, 0 }, // position
		{ 0, 0 ,1 }, //direction
		0.0f, // yaw rotation
		60.0f, // FOV
		1.0f, // NEAR
		100.0f // FAR
	);

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
	draw.draw_circle(10, 10, 100, C_RED);
}

int main(int argc, char* argv[]) {
	Example example;
	example.run();
	return 0;
}