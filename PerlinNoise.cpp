#define DGE_APPLICATION
#include "defGameEngine.h"

constexpr float BOUND = (3.14159265 / ~(~0u >> 1));

class Example : public def::GameEngine
{
public:
	Example()
	{
		SetTitle("Example");
	}

protected:
	def::vf2d RandGradient(const def::vi2d& pos)
	{
		uint32_t width = 8 * sizeof(uint32_t);
		uint32_t rot = width / 2;

		uint64_t x = (uint64_t)pos.x;
		uint64_t y = (uint64_t)pos.y;

		x *= 3284157443;
		y ^= x << rot | x >> width - rot;

		y *= 1911520717;
		x ^= y << rot | y >> width - rot;

		x *= 2048419325;

		float rand = (float)x * BOUND;
		return { cosf(rand), sinf(rand) };
	}

	float DotGradient(const def::vi2d& posI, const def::vf2d& posF)
	{
		def::vf2d grad = RandGradient(posI);
		def::vf2d dist = posF - def::vf2d(posI);

		return dist.dot(grad);
	}

	float PerlinNoise2D(const def::vf2d& pos)
	{
		auto lerp = [](float start, float end, float t)
		{
			return (end - start) * t + start;
		};

		def::vi2d p0 = pos.floor();
		def::vi2d p1 = def::vi2d(pos) + def::vi2d(1, 1);

		def::vf2d s = pos - def::vf2d(p0);

		float n0, n1, ix0, ix1;

		n0 = DotGradient(p0, pos);
		n1 = DotGradient({ p1.x, p0.y }, pos);
		ix0 = lerp(n0, n1, s.x);

		n0 = DotGradient({ p0.x, p1.y }, pos);
		n1 = DotGradient(p1, pos);
		ix1 = lerp(n0, n1, s.x);

		return lerp(ix0, ix1, s.y) * 0.5f + 0.5f;
	}

	void ApplyPerlinNoise2D()
	{
		for (int32_t x = 0; x < ScreenWidth(); x++)
			for (int32_t y = 0; y < ScreenHeight(); y++)
			{
				size_t i = y * ScreenWidth() + x;
				perlinNoise[i] = PerlinNoise2D(def::vf2d(x, y) / def::vf2d(ScreenSize())) * factor;
			}
	}

public:
	bool OnUserCreate() override
	{
		perlinNoise = new float[ScreenWidth() * ScreenHeight()];

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetKey(def::Key::UP).held) factor += deltaTime;
		if (GetKey(def::Key::DOWN).held) factor -= deltaTime;

		auto start = std::chrono::high_resolution_clock::now();
		ApplyPerlinNoise2D();
		auto end = std::chrono::high_resolution_clock::now();

		double elapsed = std::chrono::duration<double>(end - start).count();

		for (int32_t x = 0; x < ScreenWidth(); x++)
			for (int32_t y = 0; y < ScreenHeight(); y++)
			{
				float n = perlinNoise[y * ScreenWidth() + x];
				Draw(x, y, def::PixelF(n, 1.0f - n, 1.0f - n));
			}

		DrawString(10, 10, "Took = " + std::to_string(elapsed) + "s");
		DrawString(10, 20, "Factor = " + std::to_string(factor));

		return true;
	}

protected:
	float* perlinNoise;
	float factor = 20.0f;

};

int main()
{
	Example demo;

	demo.Construct(256, 240, 4, 4);
	demo.Run();

	return 0;
}