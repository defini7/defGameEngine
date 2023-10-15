#define DGE_APPLICATION
#include "defGameEngine.hpp"

constexpr size_t STARS_COUNT = 200;

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Space");
	}

	struct Star
	{
		float angle;
		float distance;
		float speed;
		def::Pixel col;
	};

	std::vector<Star> stars;

	def::vf2d origin;
	float bound;

protected:
	float RandFloat(float min, float max)
	{
		return min + (max - min) * ((float)rand() / (float)RAND_MAX);
	}

	void AddStar(size_t i)
	{
		stars[i].angle = RandFloat(0.0f, 2.0f * 3.14159f);
		stars[i].speed = RandFloat(10.0f, 100.0f);
		stars[i].distance = RandFloat(bound / 10.0f * 0.5f, bound * 0.5f);
		
		float lum = RandFloat(0.5f, 1.0f);
		stars[i].col = def::PixelF(lum, lum, lum);
	}

	bool OnUserCreate() override
	{
		origin = ScreenSize() / 2;
		bound = float(std::max(ScreenWidth(), ScreenHeight())) * 0.8f;

		stars.resize(STARS_COUNT);

		for (size_t i = 0; i < STARS_COUNT; i++)
			AddStar(i);

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		Clear(def::BLACK);

		for (size_t i = 0; i < STARS_COUNT; i++)
		{
			auto& s = stars[i];
			s.distance += s.speed * deltaTime * (s.distance / (bound * 0.5f));

			if (s.distance > bound)
				AddStar(i);

			Draw(origin + def::vf2d(cos(s.angle), sin(s.angle)) * s.distance, s.col * (s.distance / bound * 0.5f));
		}

		return true;
	}
};

int main()
{
	Sample demo;

	demo.Construct(256, 240, 4, 4);
	demo.Run();

	return 0;
}
