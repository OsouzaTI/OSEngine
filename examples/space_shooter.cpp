#include "renderer.h"

class Example : public OSRenderer
{
public:
	Example() {};
	~Example() {};

	std::vector<vect3<float>> stars;

	void engine_main() override;
	void process_input() override;
	void update() override;
	void render() override;

	void create_stars(int number_of_stars);
	void update_stars(float velocity);

private:

};

float original_camera_y;
int number_of_objects_star = 10;

void Example::engine_main()
{

	create_window("Example window", 600, 600, NONGUI);
	
	create_stars(100);

	create_camera(
		{ 0, 0, 0 },
		{ 0, 0 ,1 },
		0.0f,
		90.0f,
		1.0f,
		200.0f
	);
	original_camera_y = Display::camera->position.y;
	Display::camera->position.x += 2;
	Display::camera->position.z = 0;

	pipeline.read_obj_file(std::string("nave"),"F:\\Projects\\cpp\\OSRenderer\\Objects3D\\obj\\f22.obj");
	pipeline.get_mesh("nave")->set_scale(vect3<float>(0.6, 0.6, 0.6));
	pipeline.get_mesh("nave")->mesh.translation.y = -3;

	pipeline.load_texture("nave","F:\\Projects\\cpp\\OSRenderer\\Objects3D\\textures\\f22.png");

	for (int i = 0; i < number_of_objects_star; i++)
	{
		char buffer[32];
		sprintf(buffer, "star %d", i);

		pipeline.read_obj_file(std::string(buffer),"F:\\Projects\\cpp\\OSRenderer\\Objects3D\\obj\\estrellica.obj");
		pipeline.get_mesh(buffer)->draw_mode = DRAW_MODE::FILLED;
		pipeline.get_mesh(buffer)->shading_type = SHADING_TYPE::FLAT;
		pipeline.get_mesh(buffer)->set_color(ImGui::ColorConvertFloat4ToU32(ImVec4{ 1, 1 , 0 , 1}));
		//pipeline.load_texture(buffer,"F:\\Projects\\cpp\\OSRenderer\\Objects3D\\textures\\kevin_go.png");

		float scale_star = 0.1f;
		pipeline.get_mesh(buffer)->set_scale(vect3<float>(scale_star, scale_star, scale_star));
		pipeline.get_mesh(buffer)->set_rotate_mesh_x(RAD(90));
		
		float x1 = (rand() % 60) + 10;
		float y1 = rand() % 60;
		float z1 = (rand() % 2)+5;
		pipeline.get_mesh(buffer)->mesh.translation.x = x1;
		pipeline.get_mesh(buffer)->mesh.translation.y = y1;
		pipeline.get_mesh(buffer)->mesh.translation.z = 20;
		

	}


}

void Example::process_input()
{
	read_event();
	switch (keyboard_event_type())
	{
	case OS_KEYBOARD_TYPE::QUIT:
		this->game_loop = false;
		break;
	default:
		break;
	}
}

float n = 0.0f;
float z = 0.0f;
float timer_rotate_x = 1.0f;
float angle_rotate_x = 0.0f;
bool rotate_x = false;
float spawn_time_star = 0;

void Example::update()
{
	frame_rate_control();
	if (n < TWO_PI) n += 1 * delta_time();
	else n = 0.0f;

	z += 1 * delta_time();

	timer_rotate_x += 1 * delta_time();
	
	if (timer_rotate_x > 10) rotate_x = true;

	if (rotate_x) {
		angle_rotate_x += 2.5f * delta_time();
		pipeline.get_mesh("nave")->set_rotate_mesh_x(angle_rotate_x);
	}

	if (angle_rotate_x > TWO_PI) {
		timer_rotate_x = 1.0f;
		angle_rotate_x = 0.0f;
		rotate_x = false;
	}

	Display::camera->position.y = original_camera_y + sin(n);
	pipeline.get_mesh("nave")->set_rotate_mesh_z(sin(-z)*0.5);	

	pipeline.process_image();
}

void Example::render() {
	clear_screen();
	// code
	update_stars(1.0f);

	pipeline.draw_pipeline();
	update_render();
}

void Example::create_stars(int number_of_stars)
{
	for (int j = 0;j < number_of_stars;j++)
	{
		int x1 = window_width() + rand() % window_width();
		int y1 = rand() % window_height();
		int size = (rand() % 5) + 2;
		vect3<float> s(x1, y1, size);
		stars.push_back(s);
	}
}

void Example::update_stars(float velocity)
{
	
	int index = -1;
	for (auto& star : stars)
	{
		index++;
		draw.draw_fill_rect(
			star.x,
			star.y,
			star.z, star.z, C_WHITE
		);
		star.x -= velocity;

		if (star.x < 0) {
			std::vector<vect3<float>> temp_stars;
			for (int i = 0; i < stars.size(); i++)
			{
				if (i != index) {
					temp_stars.push_back(stars[i]);
				}
			}
			stars = temp_stars;
			create_stars(1);
		}

	}

	// loop the meshs star
	for (int i = 0; i < number_of_objects_star; i++)
	{
		char buffer[32];
		sprintf(buffer, "star %d", i);
		Mesh* temp = pipeline.get_mesh(buffer);
		if (temp) {
			temp->mesh.translation.x -= velocity * delta_time() * 2;

			if (temp->mesh.translation.x < -35) {
				temp->mesh.translation.x += 60;
				temp->mesh.translation.y = rand() % 60;
			}

		}
	}
}

int main(int argc, char* argv[]) {
	Example example;
	example.run();
	return 0;
}