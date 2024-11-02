#define DGE_APPLICATION
#include "../defGameEngine.hpp"

#include <algorithm>
#include <array>

constexpr int nPaletteWidth = 8;
constexpr int nPaletteHeight = 8;

constexpr int nColorWidth = 8;
constexpr int nColorHeight = 8;

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Sample");
	}

private:
	std::array<def::Pixel, nPaletteWidth * nPaletteHeight> palette;

	def::Pixel* pSelected = nullptr;

	const def::vi2d vColorSize = { nColorWidth, nColorHeight };

protected:
	bool OnUserCreate() override
	{
		for (int i = 0; i < nPaletteWidth * nPaletteHeight; i++)
			palette[i] = def::Pixel(rand() % 256, rand() % 256, rand() % 256);

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		def::vi2d vMouse = GetMousePos();
		def::vi2d vPalette = vMouse / vColorSize;

		// Find selected color

		bool bSelected = false;
		for (int i = 0; i < nPaletteWidth; i++)
			for (int j = 0; j < nPaletteHeight; j++)
			{
				if (vPalette == def::vi2d(i, j))
				{
					pSelected = &palette[j * nPaletteWidth + i];
					bSelected = true;
				}
			}

		// If nothing was selected,
		// we erase previous selected colour
		if (!bSelected) pSelected = nullptr;

		// DRAWING ROUTINES

		// Clear screen
		Clear(def::WHITE);

		// Draw palette

		for (int i = 0; i < nPaletteWidth; i++)
			for (int j = 0; j < nPaletteHeight; j++)
			{
				def::vi2d vPos = def::vi2d(i, j) * vColorSize;

				FillRectangle(vPos, vColorSize, palette[j * nPaletteWidth + i]);
				DrawRectangle(vPos, vColorSize, def::BLACK);
			}

		if (pSelected != nullptr)
		{
			// Draw selected color

			def::vi2d vSelectedPos = vPalette * vColorSize;
			def::vi2d vSelectedSize = vColorSize * 2;

			FillRectangle(vSelectedPos, vSelectedSize, *pSelected);
			DrawRectangle(vSelectedPos, vSelectedSize, def::BLACK);

			std::string sColor = std::to_string((int)pSelected->r) + ", " + std::to_string((int)pSelected->g) + ", " + std::to_string((int)pSelected->b);

			DrawString(100, 10, sColor, def::BLACK);
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
