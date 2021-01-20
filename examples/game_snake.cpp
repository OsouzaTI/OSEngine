#include <iostream>
#include "../os_engine.hpp"
#include <vector>
#include <cstdint>
#include <memory>
#include <string>
#include <fstream>

#define LOG(x) std::cout << x << std::endl;
#define GRID_SIZE 20
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

AreaLogger logger;

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

	bool DetectCollision(int xx, int yy, Snake* snk) {
		int GridCount = (int)(WINDOW_WIDTH / GRID_SIZE);

		if (x == xx && y == yy) {
			snk->tail++;
			snk->pts++;						
			x = rand() % GridCount;
			y = rand() % GridCount - (4);
			logger.add_log(STR("food: <x: ") + NSTR(x) + STR(", y: ") + NSTR(y) + STR(">"));
			return true;
		}
		return false;
	}

};

class SnakeGame : public OSEngine
{

	public:
		SnakeGame();
		~SnakeGame();

		// label score
		TextLabel score;

		void save_score();

		void SnakeUpdate();
		void engine_main() override;
		void process_input() override;
		void update() override;
		void render() override;
	private:
		   Snake snk;
		   FoodSnake fsnk{ 20, 20 };
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

	if (fsnk.DetectCollision(snk.posx, snk.posy, &snk)) {
		std::string _score = std::string("Score: ") + std::to_string(snk.pts);
		score.set_text(_score.c_str());
	}

	if (snk.DetectSelfCollision()) {
		save_score();
		snk.pts = 0;
	}
		
}

void SnakeGame::engine_main()
{
	create_window("Janela", WINDOW_WIDTH, WINDOW_HEIGHT);

	score.GUI_init("Score: 0", 10, 10, 20);
	logger.GUI_init(0, -(100 - window_height()), 300, 100);
	logger.lock_view_end(true);

	set_frame_rate(15);	
	set_background_color(C_BLUE);

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

	score.GUI_draw();
	logger.GUI_draw();

	update_render();
}



int main(int argc, char* argv[]) {
	SnakeGame game;
	game.engine_main();
	return 0;
}