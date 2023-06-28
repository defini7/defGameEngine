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

#include <array>

using namespace std;

constexpr int MAP_WIDTH = 3;
constexpr int MAP_HEIGHT = 3;

constexpr int CELL_SIZE = 16;

template <typename T>
struct Map
{
	Map() = default;
	~Map() = default;

	array<T, MAP_WIDTH * MAP_HEIGHT> values;

	void set(int x, int y, T value)
	{
		if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT)
			values[y * MAP_WIDTH + x] = value;
	}

	T* get(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT)
			return &values[y * MAP_WIDTH + x];

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

	def::vi2d mapSize = { MAP_WIDTH, MAP_HEIGHT };
	def::vi2d cellSize = { CELL_SIZE, CELL_SIZE };

public:
	bool OnUserCreate() override
	{
		srand(time(0));

		for (int i = 0; i < map.values.size() - 1; i++)
		{
			int value;

			do { value = rand() % (map.values.size() + 1); }
			while (
				find(map.values.begin(), map.values.end(), value) != map.values.end() ||
				value == map.values.size()
			);

			map.values[i] = value;
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetMouse(0).pressed)
		{
			def::vi2d cellPos = GetMouse() / cellSize;

			if (cellPos < mapSize && cellPos >= def::vi2d(0, 0))
			{
				int* valueOld = map.get(cellPos.x, cellPos.y);

				auto check = [&](int ox, int oy)
				{
					int* valueNew = map.get(cellPos.x + ox, cellPos.y + oy);
					if (valueNew != nullptr && *valueNew == 0)
					{
						*valueNew = *valueOld;
						*valueOld = 0;

						return true;
					}

					return false;
				};

				if (check(-1, 0)) {}
				else if (check(0, -1)) {}
				else if (check(1, 0)) {}
				else if (check(0, 1)) {}
			}
		}

		Clear(def::BLACK);

		def::vi2d pos;
		for (pos.x = 0; pos.x < MAP_WIDTH; pos.x++)
			for (pos.y = 0; pos.y < MAP_HEIGHT; pos.y++)
			{
				def::vi2d cellPos = pos * cellSize;
				int value = map.values[pos.y * MAP_WIDTH + pos.x];

				DrawRectangle(cellPos, cellSize);
				if (value > 0) DrawString(def::vf2d(cellPos) + def::vf2d(cellSize) * 0.25f, std::to_string(value));
			}

		return true;
	}
};

int main()
{
	SlidingPuzzle demo;
	demo.Construct(256, 240, 4, 4);
	demo.Run();
	return 0;
}
