/*
* BSD 3-Clause License
Copyright (c) 2023, Алекс
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

#include "defGameEngine.h"

#include <array>

using namespace std;

constexpr int nMapWidth = 3;
constexpr int nMapHeight = 3;

constexpr int nCellSize = 16;

template <typename T>
struct Map
{
	Map() = default;
	~Map() = default;

	array<T, nMapWidth* nMapHeight> values;

	void set(int x, int y, T value)
	{
		if (x >= 0 && y >= 0 && x < nMapWidth && y < nMapHeight)
			values[y * nMapWidth + x] = value;
	}

	T* get(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < nMapWidth && y < nMapHeight)
			return &values[y * nMapWidth + x];

		return nullptr;
	}
};

class SlidingPuzzle : public def::GameEngine
{
public:
	SlidingPuzzle()
	{
		SetTitle("Sliding Puzzle");
	}

private:
	Map<int> map;

	def::vi2d vMapSize = { nMapWidth, nMapHeight };
	def::vi2d vCellSize = { nCellSize, nCellSize };

public:
	bool OnUserCreate() override
	{
		srand(time(0));

		for (int i = 0; i < map.values.size() - 1; i++)
		{
			int nValue;

			do { nValue = rand() % (map.values.size() + 1); }
			while (
				find(map.values.begin(), map.values.end(), nValue) != map.values.end() ||
				nValue == map.values.size()
			);

			map.values[i] = nValue;
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetMouse(0).bPressed)
		{
			def::vi2d vCellPos = GetMouse() / vCellSize;

			if (vCellPos < vMapSize && vCellPos >= def::vi2d(0, 0))
			{
				int* pValueOld = map.get(vCellPos.x, vCellPos.y);

				auto check = [&](int ox, int oy)
				{
					int* pValueNew = map.get(vCellPos.x + ox, vCellPos.y + oy);
					if (pValueNew != nullptr && *pValueNew == 0)
					{
						*pValueNew = *pValueOld;
						*pValueOld = 0;

						return true;
					}

					return false;
				};

				if (check(-1, 0)) goto out;
				if (check(0, -1)) goto out;
				if (check(1, 0)) goto out;
				if (check(0, 1)) goto out;
			}
		}

	out:

		Clear(def::BLACK);

		def::vi2d vPos;
		for (vPos.x = 0; vPos.x < nMapWidth; vPos.x++)
			for (vPos.y = 0; vPos.y < nMapHeight; vPos.y++)
			{
				def::vi2d vCellPos = vPos * vCellSize;
				int nValue = map.values[vPos.y * nMapWidth + vPos.x];

				DrawRectangle(vCellPos, vCellSize);

				if (nValue > 0)
					DrawString(def::vf2d(vCellPos) + def::vf2d(vCellSize) * 0.25f, std::to_string(nValue));
			}

		return true;
	}
};

int main()
{
	SlidingPuzzle demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Run();
	return 0;
}
