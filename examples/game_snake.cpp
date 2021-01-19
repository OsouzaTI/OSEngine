#include <iostream>
#include "../os_engine.hpp"
#include <vector>
#include <cstdint>
#include <memory>
#include <string>
#include <fstream>

#define LOG(x) std::cout << x << std::endl;
#define GRID_SIZE 20
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

struct SnakeBody
{
	int x;
	int y;
};


struct Snake {

	int posx = 10;
	int posy = 10;
	int velx = 0;
	int vely = 0;
	int tail = 5;
	int pts = 0;
	std::vector<SnakeBody> trilha;

	void UpdatePosition() {
		posx += velx;
		posy += vely;
	}

	void LimitScreen() {
		int GridCount = (int)(WINDOW_WIDTH / GRID_SIZE);

		if (posx >= GridCount) {
			posx = 0;
		}

		if (posx < 0) {
			posx = GridCount - 1;
		}

		if (posy >= GridCount) {
			posy = 0;
		}

		if (posy < 0) {
			posy = GridCount - 1;
		}
	}

	void AddSnake(int x, int y) {
		trilha.push_back({ x, y });
	}

	void RemoveExcess() {
		while (trilha.size() > tail) trilha.erase(trilha.begin());
	}

	void ChangeDirection(int vx, int vy) {
		// Y
		if (vx == 0) {
			if (abs(vy) == abs(vely)) return;
		}
		// X
		else {
			if (abs(vx) == abs(velx)) return;
		}

		velx = vx;
		vely = vy;

	}

	bool DetectSelfCollision() {
		SnakeBody head = trilha.at(0);
		for (int i = 1; i < trilha.size(); i++) {
			if (head.x == trilha[i].x && head.y == trilha[i].y) {
				tail = 5;				
				if(trilha.size() > 5) return true;
			}
		}
		return false;
	}

};

struct FoodSnake {
	int x;
	int y;

	void DetectCollision(int xx, int yy, Snake* snk) {
		int GridCount = (int)(WINDOW_WIDTH / GRID_SIZE);

		if (x == xx && y == yy) {
			snk->tail++;
			snk->pts++;
			std::cout << snk->pts;
			x = rand() % GridCount;
			y = rand() % GridCount;
		}

	}

};

class SnakeGame : public OSEngine
{

	public:
		SnakeGame();
		~SnakeGame();

		void save_score();

		void SnakeUpdate();
		void engine_main() override;
		void process_input() override;
		void update() override;
		void render() override;
	private:
		   Snake snk;
		   FoodSnake fsnk{ 0, 0 };
};

SnakeGame::SnakeGame(){}
SnakeGame::~SnakeGame(){}

void SnakeGame::save_score()
{
	std::ofstream file_score;
	file_score.open("score.txt", std::ios::app);
	file_score << "Score: " << snk.pts << std::endl;
	file_score.close();
}

void SnakeGame::SnakeUpdate()
{
	snk.UpdatePosition();

	for (int i = 0; i < snk.trilha.size(); i++)
	{
		draw.draw_fill_rect(
			snk.trilha[i].x * GRID_SIZE,
			snk.trilha[i].y * GRID_SIZE,
			GRID_SIZE - 2,
			GRID_SIZE - 2,
			0xFFFFFF00);
	}

	snk.LimitScreen();
	snk.AddSnake(snk.posx, snk.posy);
	snk.RemoveExcess();

	draw.draw_fill_rect(
		fsnk.x * GRID_SIZE,
		fsnk.y * GRID_SIZE,
		GRID_SIZE - 2,
		GRID_SIZE - 2,
		0xFFFF0000);

	fsnk.DetectCollision(snk.posx, snk.posy, &snk);
	if (snk.DetectSelfCollision()) {
		save_score();
		snk.pts = 0;
	}
}

void SnakeGame::engine_main()
{
	create_window("Janela", WINDOW_WIDTH, WINDOW_HEIGHT);
	set_frame_rate(15);	
	vect2<float> nota = { 10, 15 };
	std::cout << nota;
	if (this->game_loop)
		std::cout << "OPA\n";

	while (this->game_loop) {
		process_input();
		update();
		render();
	}
}

void SnakeGame::process_input()
{
	read_event();
	switch (keyboard_event_type())
	{
		case OS_KEYBOARD_TYPE::QUIT:
			this->game_loop = false;
			break;
		case OS_KEYBOARD_TYPE::KEY_DOWN:
			switch (keyboard_event_key())
			{
				case OS_KEYBOARD_KEY::BT_ARROW_LEFT:  snk.ChangeDirection(-1, 0); break;
				case OS_KEYBOARD_KEY::BT_ARROW_RIGHT: snk.ChangeDirection(1, 0);  break;
				case OS_KEYBOARD_KEY::BT_ARROW_UP:    snk.ChangeDirection(0, -1); break;
				case OS_KEYBOARD_KEY::BT_ARROW_DOWN:  snk.ChangeDirection(0, 1);  break;
				default: break;
			}
		default:
			break;
	}
}

void SnakeGame::update()
{
	frame_rate_control();
}

void SnakeGame::render()
{
	clear_screen();

	SnakeUpdate();

	draw_buffer();
	update_render();
}



int main(int argc, char* argv[]) {
	SnakeGame game;
	game.engine_main();
	return 0;
}