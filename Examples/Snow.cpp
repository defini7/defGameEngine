#define DGE_APPLICATION
#include "defGameEngine.hpp"

class Snow : public def::GameEngine
{
public:
	Snow()
	{
		SetTitle("Snow");
	}

private:
	struct Flake
	{
		def::vf2d pos;
	
		float time = 0.0f;
		float speed = 1.0f;
	};

	std::list<Flake> flakes;
	int flakeRadius = 4;

	float speedDiff = 0.0f;

private:
	float rnd(float min, float max)
	{
		return (float)rand() / (float)RAND_MAX * (max - min) + min;
	}

protected:
	bool OnUserCreate() override
	{
		srand(time(nullptr));

		flakes.resize(1024);

		for (auto& f : flakes)
		{
			f.pos = { rnd(0.0f, ScreenWidth()), float(-flakeRadius) };
			f.speed = rnd(100.0f, 200.0f);
		}

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetKey(def::Key::UP).held)
			speedDiff += deltaTime * 100.0f;

		if (GetKey(def::Key::DOWN).held)
			speedDiff -= deltaTime * 100.0f;

		speedDiff = std::max(0.0f, speedDiff);

		for (auto& f : flakes)
		{
			f.time += deltaTime;
			f.pos.y = f.time * (f.speed + speedDiff);

			if (f.pos.y >= ScreenHeight() + flakeRadius)
			{
				f.pos = { rnd(-speedDiff / 10.0f, ScreenWidth()), float(-flakeRadius) };
				f.time = 0.0f;
			}
		}

		Clear(def::BLACK);

		for (auto& f : flakes)
		{
			f.pos.x += deltaTime * speedDiff / 10.0f;

			float c = abs(1.0f - 1.0f / f.speed * 200.0f);
			FillCircle(f.pos + def::vf2d(f.pos.x + cos(f.time * f.speed * 0.1f), 0.0f), (float)flakeRadius * c, def::PixelF(c, c, c));
		}

		return true;
	}

};

int main()
{
	Snow app;

	app.Construct(1280, 720, 1, 1);
	app.Run();

	return 0;
}
