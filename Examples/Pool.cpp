#define DGE_APPLICATION
#include "defGameEngine.hpp"

#include <list>

constexpr float BALL_MASS = 284.0f;
constexpr float BALL_RADIUS = 20.0f;
constexpr float BORDER_WIDTH = 30.0f;
constexpr float POCKET_RADIUS = BORDER_WIDTH * 2.5f;
constexpr float FORCE_ACC = 5.0f;

struct Ball
{
	enum Type
	{
		WHITE,
		BLACK,
		POCKET
	};

	uint8_t type;
	uint8_t id;

	def::vf2d acc;
	def::vf2d vel;
	def::vf2d pos;

	float mass;
	float radius;

	bool inHole = false;

	bool IsOverlap(Ball& b)
	{
		return fabs((pos - b.pos).mag2()) <= pow(radius + b.radius, 2);
	}

	bool IsInside(const def::vf2d& p)
	{
		return fabs((pos - p).mag2()) <= pow(radius, 2);
	}

	void Update(const float deltaTime, const def::vf2d& tl, const def::vf2d& br, const float bw)
	{
		acc = vel * -0.98f;

		vel += acc * deltaTime;
		pos += vel * deltaTime;

		if (fabs(vel.mag2()) < 0.01f)
			vel = { 0.0f, 0.0f };
	}

};

struct Table
{
	Table() = default;
	Table(const def::vf2d& tl, const def::vf2d& br, const float brdWidth, const float pctRadius)
		: boundary{ tl, br }, borderWidth(brdWidth) {}

	void AddBall(const def::vf2d& pos, const float radius, const uint8_t type = Ball::WHITE)
	{
		Ball b;
		b.pos = pos;
		b.radius = radius;
		b.mass = BALL_MASS;
		b.type = type;
		b.id = balls.size();

		if (type == Ball::BLACK)
			balls.push_front(b);
		else
			balls.push_back(b);
	}

	void Clear()
	{
		balls.clear();
	}

	void UpdateBalls(const float deltaTime, int& score)
	{
		for (auto& ball : balls)
			ball.Update(deltaTime, boundary.first, boundary.second, borderWidth);

		auto removeAt = std::remove_if(balls.begin(), balls.end(), [](const Ball& b) { return b.inHole; });
		if (removeAt != balls.end()) balls.erase(removeAt);

		if (balls.empty())
		{
			// If there is no balls then assume, that they failed,
			// because there must be at least a black ball
			gameOver = true;
			won = false;
		}
		else
		{
			// If there is only one ball...
			if (balls.size() == 1)
			{
				gameOver = true;

				// ...and it's black
				won = (balls.back().type == Ball::BLACK);
			}
			else
			{
				if (balls.front().type != Ball::BLACK)
				{
					// They hit a black ball in a pocket
					gameOver = true;
					won = false;
				}
			}

			// if there is more than one ball then we don't care
		}

		std::vector<std::pair<Ball*, Ball*>> collidingPairs;
		StaticCollision(collidingPairs, score);
		DynamicCollision(collidingPairs);
	}

	void UpdateCue(def::GameEngine* dge)
	{
		if (dge->GetMouse(def::Button::LEFT).pressed)
		{
			selected = nullptr;
			for (auto& ball : balls)
			{
				if (ball.type == Ball::BLACK && ball.IsInside(dge->GetMousePos()))
				{
					selected = &ball;
					break;
				}
			}
		}

		if (dge->GetMouse(def::Button::LEFT).released)
		{
			if (selected)
				selected->vel = FORCE_ACC * (selected->pos - def::vf2d(dge->GetMousePos()));

			selected = nullptr;
		}
	}

	void Update(def::GameEngine* dge, const float deltaTime, int& score)
	{
		UpdateBalls(deltaTime, score);
		UpdateCue(dge);
	}

	void StaticCollision(std::vector<std::pair<Ball*, Ball*>>& collidingPairs, int& score)
	{
		for (auto& ball : balls)
		{
			for (auto& target : balls)
			{
				if (ball.id != target.id)
				{
					if (ball.IsOverlap(target))
					{
						if (ball.type == Ball::POCKET)
						{
							target.inHole = true;
							score += (target.type == Ball::WHITE) ? 1 : -1;
							if (score < 0) score = 0;
						}
						else if (target.type == Ball::POCKET) {}
						else
						{
							collidingPairs.push_back({ &ball, &target });

							float dist = (ball.pos - target.pos).mag();
							float overlap = 0.5f * (dist - ball.radius - target.radius);

							def::vf2d vel = overlap * (ball.pos - target.pos) / dist;

							ball.pos -= vel;
							target.pos += vel;
						}
					}
				}
			}

			if (ball.type != Ball::POCKET)
			{
				if (
					ball.pos.y < boundary.first.y + borderWidth + ball.radius ||
					ball.pos.y > boundary.second.y - borderWidth - ball.radius
					)
					ball.vel.y = -ball.vel.y;

				if (
					ball.pos.x < boundary.first.x + borderWidth + ball.radius ||
					ball.pos.x > boundary.second.x - borderWidth - ball.radius
					)
					ball.vel.x = -ball.vel.x;
			}
		}
	}

	void DynamicCollision(const std::vector<std::pair<Ball*, Ball*>>& collidingPairs)
	{
		for (auto& pair : collidingPairs)
		{
			Ball* b1 = pair.first;
			Ball* b2 = pair.second;

			float dist = (b1->pos - b2->pos).mag();

			def::vf2d norm = (b2->pos - b1->pos) / dist;

			def::vf2d k = b1->vel - b2->vel;
			float p = 2.0f * norm.dot(k) / (b1->mass + b2->mass);
			b1->vel -= p * b2->mass * norm;
			b2->vel += p * b1->mass * norm;
		}
	}

	void DrawBalls(def::GameEngine* dge)
	{
		for (const auto& ball : balls)
		{
			def::Pixel col;

			switch (ball.type)
			{
			case Ball::WHITE: col = def::WHITE; break;
			case Ball::BLACK: col = def::BLACK; break;
			case Ball::POCKET: col = def::DARK_BROWN; break;
			}

			dge->FillCircle(ball.pos, ball.radius, col);
		}
	}

	void DrawCue(def::GameEngine* dge, const def::Pixel& col = def::DARK_GREY)
	{
		if (selected)
			dge->DrawLine(selected->pos, dge->GetMousePos(), col);
	}

	void DrawBoundary(def::GameEngine* dge, const def::Pixel& col = def::BROWN)
	{
		auto& b = boundary;
		dge->FillRectangle(b.first, def::vi2d(b.second.x - b.first.x - borderWidth, borderWidth), col);
		dge->FillRectangle(b.second.x - borderWidth, b.first.y, borderWidth, b.second.y - b.first.y - borderWidth, col);
		dge->FillRectangle(b.first.x, b.first.x + borderWidth, borderWidth, b.second.y - b.first.y - borderWidth, col);
		dge->FillRectangle(b.first.x + borderWidth, b.second.y - borderWidth, b.second.x - b.first.x - borderWidth, borderWidth, col);
	}

	void Draw(def::GameEngine* dge)
	{
		DrawBalls(dge);
		DrawBoundary(dge);
		DrawCue(dge);
	}

	Ball* selected = nullptr;
	std::list<Ball> balls;

	std::pair<def::vf2d, def::vf2d> cuePos;
	std::pair<def::vf2d, def::vf2d> boundary;

	float borderWidth;

	bool won = false;
	bool gameOver = false;
};

class Pool : public def::GameEngine
{
public:
	Pool()
	{
		SetTitle("Pool");
	}

	~Pool()
	{
		delete table;
	}

	Table* table = nullptr;
	int score = 0;

	def::vf2d topLeft;
	def::vf2d bottomRight;

	void ResetTable(const def::vf2d& tl, const def::vf2d& br, const float borderWidth = BORDER_WIDTH, const float ballRadius = BALL_RADIUS, const float pocketRadius = POCKET_RADIUS)
	{
		table->Clear();

		table->AddBall(def::vf2d(ScreenWidth() * 0.4f + ballRadius * 4 + 4, ScreenHeight() - borderWidth * 5), ballRadius, Ball::BLACK);

		table->AddBall(def::vf2d(ScreenWidth() * 0.4f, borderWidth * 5), ballRadius);
		for (int i = 0; i < 4; i++)
			table->AddBall(table->balls.back().pos + def::vf2d(ballRadius * 2 + 2, 0), ballRadius);

		table->AddBall(def::vf2d(ScreenWidth() * 0.4f + ballRadius + 1, borderWidth * 5 + ballRadius * 2 + 2), ballRadius);
		for (int i = 0; i < 3; i++)
			table->AddBall(table->balls.back().pos + def::vf2d(ballRadius * 2 + 2, 0), ballRadius);

		table->AddBall(def::vf2d(ScreenWidth() * 0.4f + ballRadius * 2 + 2, borderWidth * 5 + ballRadius * 4 + 4), ballRadius);
		for (int i = 0; i < 2; i++)
			table->AddBall(table->balls.back().pos + def::vf2d(ballRadius * 2 + 2, 0), ballRadius);

		table->AddBall(def::vf2d(ScreenWidth() * 0.4f + ballRadius * 3 + 3, borderWidth * 5 + ballRadius * 6 + 6), ballRadius);
		table->AddBall(table->balls.back().pos + def::vf2d(ballRadius * 2 + 2, 0), ballRadius);

		table->AddBall(def::vf2d(ScreenWidth() * 0.4f + ballRadius * 4 + 4, borderWidth * 5 + ballRadius * 8 + 8), ballRadius);

		AddPockets(tl, br, pocketRadius);
	}

	void AddPockets(const def::vf2d& tl, const def::vf2d& br, const float radius = POCKET_RADIUS)
	{
		table->AddBall(tl, radius, Ball::POCKET);
		table->AddBall(br, radius, Ball::POCKET);
		table->AddBall({ tl.x, br.y }, radius, Ball::POCKET);
		table->AddBall({ br.x, tl.y }, radius, Ball::POCKET);
	}

	bool IsGameOver()
	{
		if (table->gameOver)
		{
			table->gameOver = false;

			if (table->won)
			{
				table->won = false;
				// TODO: Add activity
			}

			return true;
		}

		return false;
	}

	void DrawScore(const def::Pixel& col = def::GREY)
	{
		DrawString(2, 2, "Score: " + std::to_string(score), col, 3, 3);
	}

private:
	bool OnUserCreate() override
	{
		topLeft = { 0.0f, 0.0f };
		bottomRight = GetScreenSize();

		table = new Table(topLeft, bottomRight, BORDER_WIDTH, POCKET_RADIUS);

		ResetTable(topLeft, bottomRight);

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (IsGameOver())
		{
			ResetTable(topLeft, bottomRight);
			score = 0;
		}

		table->Update(this, deltaTime, score);

		Clear(def::DARK_GREEN);

		table->Draw(this);
		DrawScore();

		return true;
	}

};

int main()
{
	Pool app;
	app.Construct(1024, 960, 1, 1);
	app.Run();
	return 0;
}
