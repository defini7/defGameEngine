#define DGE_APPLICATION
#include "../defGameEngine.hpp"

using namespace def;

vf2d RandomGradient(const vi2d& i)
{
	uint32_t a = i.x, b = i.y;
	a *= 3284157443u; b ^= a << 16 | a >> 16;
	b *= 1911520717u; a ^= b << 16 | b >> 16;
	a *= 2048419325u;

	float random = (float)a * 3.14159f / float(~(~0u >> 1));
	return { cosf(random), sinf(random) };
}

float DotProductGridGradient(const vi2d& i, const vf2d& p)
{
	return (p - vf2d(i)).dot(RandomGradient(i));
}

// Returns float value (0.0f - 1.0f)
float PerlinNoise2D(const vf2d& p)
{
	vi2d i0 = p.floor();
	vi2d i1 = i0 + 1.0f;

	vf2d w = p - vf2d(i0);

	float ix0 = std::lerp(
		DotProductGridGradient(i0, p),
		DotProductGridGradient({ i1.x, i0.y }, p),
		w.x);

	float ix1 = std::lerp(
		DotProductGridGradient({ i0.x, i1.y }, p),
		DotProductGridGradient(i1, p),
		w.x);

	return std::lerp(ix0, ix1, w.y);
}

class PerlinNoise : public def::GameEngine
{
public:
	PerlinNoise()
	{
		SetTitle("Perlin Noise");
	}

private:
	void UpdateMap()
	{
		for (int i = 0; i < ScreenWidth(); i++)
			for (int j = 0; j < ScreenHeight(); j++)
			{
				float n = 0.0f;

				float frequency = 1.0f;
				float amplitude = 1.0f;

				for (int o = 0; o < octaves; o++)
				{
					n += PerlinNoise2D(vf2d(i, j) / (vf2d)GetScreenSize() * frequency) * amplitude;

					frequency *= 2.0f;
					amplitude *= 0.5f;
				}

				map[j * ScreenWidth() + i] = std::clamp(n * 1.2f, -1.0f, 1.0f) * 0.5f + 0.5f;
			}
	}

private:
	int octaves = 12;
	float* map = nullptr;

protected:
	bool OnUserCreate() override
	{
		map = new float[ScreenWidth() * ScreenHeight()];
		memset(map, 0, sizeof(float) * ScreenWidth() * ScreenHeight());

		UpdateMap();

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetKey(def::Key::LEFT).pressed) octaves--;
		if (GetKey(def::Key::RIGHT).pressed) octaves++;
		octaves = std::clamp(octaves, 0, 32);

		if (GetKey(def::Key::SPACE).pressed)
			UpdateMap();

		for (int i = 0; i < ScreenWidth(); i++)
			for (int j = 0; j < ScreenHeight(); j++)
			{
				float n = map[j * ScreenWidth() + i];
				uint8_t col = uint8_t(n * 12.0f);

				def::Pixel pix;

				switch (col)
				{
				case 0: pix = BLACK; break;

				case 1: pix = DARK_BROWN; break;
				case 2: pix = BROWN; break;
				case 3: pix = DARK_RED; break;
				case 4: pix = RED; break;

				case 5: pix = DARK_PURPLE; break;
				case 6: pix = DARK_BLUE; break;
				case 7: pix = PURPLE; break;
				case 8: pix = BLUE; break;

				case 9:  pix = DARK_ORANGE; break;
				case 10: pix = ORANGE; break;
				case 11: pix = BEIGE; break;
				case 12: pix = YELLOW; break;
				}

				Draw(i, j, pix);
			}

		DrawString(2, 2, "Octaves = " + std::to_string(octaves), def::CYAN);

		return true;
	}

};

int main()
{
	PerlinNoise demo;

	demo.Construct(1280, 720, 1, 1);
	demo.Run();

	return 0;
}
