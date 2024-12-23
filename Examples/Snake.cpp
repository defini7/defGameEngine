#define DGE_APPLICATION
#include "../defGameEngine.hpp"

#include <list>

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Sample");
	}

protected:
	def::vi2d apple;
	int dir;
	bool isDead;

	int score = 0;
	int record = 0;

	std::list<def::vi2d> snake;

	def::vi2d bodyPartSize = { 16, 16 };
	def::vi2d worldSize;

	float delay = 0.5f;
	float timer = 0.0f;

protected:
	bool OnUserCreate() override
	{
		worldSize = GetScreenSize() / bodyPartSize;

		NewGame();

		return true;
	}

	void NewGame()
	{
		isDead = false;

		apple = worldSize / def::vi2d(4, 2);
		dir = 0;

		def::vi2d snakePos = worldSize / 2;
		snake =
		{
			snakePos,
			snakePos + def::vi2d(1, 0),
			snakePos + def::vi2d(2, 0),
			snakePos + def::vi2d(3, 0),
		};

		if (record < score)
			record = score;

		score = 0;

		srand(time(0));
	}

	void DrawBodyPart(const def::vi2d& pos, const def::Pixel& col)
	{
		FillRectangle(pos * bodyPartSize, bodyPartSize, col);
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (isDead)
		{
			if (GetKey(def::Key::SPACE).pressed)
				NewGame();

			DrawString(GetScreenSize() / def::vi2d(3, 2), "GAME OVER! PRESS SPACE.");
			return true;
		}

		if (GetKey(def::Key::A).pressed) dir = 0;
		if (GetKey(def::Key::W).pressed) dir = 1;
		if (GetKey(def::Key::D).pressed) dir = 2;
		if (GetKey(def::Key::S).pressed) dir = 3;

		if (timer >= delay)
		{
			switch (dir)
			{
			case 0: snake.push_front({ snake.front().x - 1, snake.front().y }); break;
			case 1: snake.push_front({ snake.front().x, snake.front().y - 1 }); break;
			case 2: snake.push_front({ snake.front().x + 1, snake.front().y }); break;
			case 3: snake.push_front({ snake.front().x, snake.front().y + 1 }); break;
			}

			if (snake.front() == apple)
			{
				score++;

				apple = def::vi2d(rand(), rand()) % worldSize;
				snake.push_back(snake.back());
			}

			for (auto i = snake.begin(); i != snake.end(); i++)
			{
				if (i != snake.begin() && *i == *snake.begin())
					isDead = true;
			}

			if (snake.front().x < 0 || snake.front().y < 0 || snake.front().x >= worldSize.x || snake.front().y >= worldSize.y)
				isDead = true;

			snake.pop_back();

			timer = 0.0f;
		}
		timer += deltaTime;

		Clear(def::GREEN);

		DrawString(0, 0, "Score: " + std::to_string(score));
		DrawString(0, 10, "Record: " + std::to_string(record));

		for (auto& s : snake)
			DrawBodyPart(s, isDead ? def::RED : def::BLUE);

		DrawBodyPart(snake.front(), isDead ? def::DARK_RED : def::WHITE);
		DrawBodyPart(apple, def::DARK_RED);

		return true;
	}
};

int main()
{
	Sample demo;

	demo.Construct(384, 240, 4, 4);
	demo.Run();

	return 0;
}
