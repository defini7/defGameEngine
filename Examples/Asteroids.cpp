#define DGE_APPLICATION
#include <defGameEngine.hpp>

#include <random>

struct Object
{
	def::vf2d pos;
	def::vf2d vel;

	int health;

	bool redundant;
};

struct Controllable : Object
{
	std::vector<def::vf2d> model;
};

struct Player : Controllable
{
	float angle;
};

bool IsPointInPolygon(const def::vf2d& polygonPos, const std::vector<def::vf2d>& polygon, const def::vf2d& pointPos)
{
	auto GetAngle = [](const def::vf2d& p1, const def::vf2d& p2)
		{
			float a = atan2(p2.y, p2.x) - atan2(p1.y, p1.x);
			while (a > 3.14159f) a -= 3.14159f * 2.0f;
			while (a < -3.14159f) a += 3.14159f * 2.0f;
			return a;
		};

	size_t points = polygon.size();

	float angle = 0.0f;

	for (size_t i = 0; i < points; i++)
		angle += GetAngle(polygonPos + polygon[i] - pointPos, polygonPos + polygon[(i + 1) % points] - pointPos);

	return std::abs(angle) >= 3.14159f;
}

class Asteroids : public def::GameEngine
{
public:
	Asteroids()
	{
		SetTitle("Asteroids");
	}

	std::mt19937 randomEngine;

	std::list<Controllable> controllables;
	std::list<Object> bullets;

	Player player;

	int score;

private:
	void CreateAsteroid(const def::vf2d& pos, const def::vf2d& vel, float radiusMultiplier = 1.0f)
	{
		Controllable go;

		go.pos = pos;
		go.vel = vel;
		go.health = 4;

		std::uniform_int_distribution<int> dist(10, 20);

		for (float angle = 0.0f; angle < 2.0f * 3.14159f; angle += 3.14159f / 6.0f)
		{
			float radius = float(dist(randomEngine)) * radiusMultiplier;
			go.model.push_back(def::vf2d(radius, angle).cart());
		}

		go.redundant = false;

		controllables.push_back(go);
	}

	void CreateBullet(const def::vf2d& pos, const def::vf2d& vel)
	{
		bullets.push_back({ pos, vel });
	}

	void WrapCoordinates(float& x, float& y)
	{
		if (x < 0.0f) x += (float)ScreenWidth();
		if (y < 0.0f) y += (float)ScreenHeight();

		if (x >= (float)ScreenWidth()) x -= (float)ScreenWidth();
		if (y >= (float)ScreenHeight()) y -= (float)ScreenHeight();
	}

	void Reset()
	{
		player.pos = GetScreenSize() / 2;
		player.vel = { 0, 0 };

		player.angle = 0.0f;

		controllables.clear();
		bullets.clear();

		randomEngine.seed(std::chrono::system_clock::now().time_since_epoch().count());

		std::uniform_int_distribution<int> distPosX(0, ScreenWidth());
		std::uniform_int_distribution<int> distPosY(0, ScreenHeight());

		std::uniform_real_distribution<float> distVel(-20.0f, 20.0f);

		for (int i = 0; i < 5; i++)
		{
			int px = distPosX(randomEngine);
			int py = distPosY(randomEngine);

			float vx = distVel(randomEngine);
			float vy = distVel(randomEngine);

			CreateAsteroid({ (float)px, (float)py }, { vx, vy });
		}

		score = 0;
	}

protected:
	bool OnUserCreate() override
	{
		player.model = 
		{
			{ 0.0f, -5.5f },
			{ -2.5f, 2.5f },
			{ 2.5f, 2.5f }
		};

		Reset();

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetKey(def::Key::A).held) player.angle -= 5.0f * deltaTime;
		if (GetKey(def::Key::D).held) player.angle += 5.0f * deltaTime;

		if (GetKey(def::Key::W).held)
		{
			player.vel.x += sinf(player.angle) * 20.0f * deltaTime;
			player.vel.y -= cosf(player.angle) * 20.0f * deltaTime;
		}

		if (GetKey(def::Key::S).held)
		{
			player.vel.x -= sinf(player.angle) * 20.0f * deltaTime;
			player.vel.y += cosf(player.angle) * 20.0f * deltaTime;
		}

		if (GetMouse(def::Button::LEFT).pressed)
			CreateBullet(player.pos, { 50.0f * sinf(player.angle), -50.0f * cosf(player.angle) });

		for (auto& b : bullets)
		{
			b.pos += b.vel * deltaTime;
			
			if (b.pos.x < 0 || b.pos.y < 0 || b.pos.x >= (float)ScreenWidth() || b.pos.y >= (float)ScreenHeight())
				b.redundant = true;

			if (!b.redundant)
			{
				for (auto& c : controllables)
				{
					if (IsPointInPolygon(c.pos, c.model, b.pos))
					{
						b.redundant = true;
						c.health--;

						score++;

						if (c.health <= 0)
						{
							c.redundant = true;

							std::uniform_real_distribution<float> dist(-20.0f, 20.0f);

							for (int i = 0; i < 4; i++)
								CreateAsteroid(b.pos, { dist(randomEngine), dist(randomEngine) }, 0.5f);
						}

						break;
					}
				}
			}
		}

		for (auto& c : controllables)
		{
			c.pos += c.vel * deltaTime;
			WrapCoordinates(c.pos.x, c.pos.y);

			for (const auto& vert : player.model)
			{
				if (IsPointInPolygon(c.pos, c.model, player.pos + vert))
				{
					Reset();
					return true;
				}
			}
		}

		player.pos += player.vel * deltaTime;
		WrapCoordinates(player.pos.x, player.pos.y);

		bullets.remove_if([](const Object& b) { return b.redundant; });
		controllables.remove_if([](const Object& c) { return c.redundant; });

		Clear(def::BLACK);

		for (const auto& b : bullets)
			Draw(b.pos.x, b.pos.y);

		for (const auto& c : controllables)
			DrawWireFrameModel(c.model, c.pos, atan2f(c.vel.y, c.vel.x) + 3.14159f * 0.5f, 1.0f, def::YELLOW);

		DrawWireFrameModel(player.model, player.pos, player.angle);

		DrawString(5, 5, "Score: " + std::to_string(score));

		return true;
	}

	bool Draw(int x, int y, const def::Pixel& col = def::WHITE) override
	{
		float nx = x;
		float ny = y;

		WrapCoordinates(nx, ny);

		return def::GameEngine::Draw(nx, ny, col);
	}
	
};

int main()
{
	Asteroids app;

	if (app.Construct(256, 240, 4, 4))
		app.Run();

	return 0;
}
