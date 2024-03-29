/*
* BSD 3-Clause License
Copyright (c) 2023, Alex
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define DGE_APPLICATION
#include "../defGameEngine.h"

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
			palette[i] = def::RandomPixel();

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		def::vi2d vMouse = GetMouse();
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
