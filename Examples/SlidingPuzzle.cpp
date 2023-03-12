#include "defGameEngine.h"

#include <array>

using namespace std;

constexpr int nMapWidth = 3;
constexpr int nMapHeight = 3;

constexpr int nCellSize = 16;

template <typename T, T junkValue>
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

	T& get(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < nMapWidth && y < nMapHeight)
			return values[y * nMapWidth + x];

		m_junk = junkValue;
		return m_junk;
	}

private:
	T m_junk;
};

class SlidingPuzzle : public def::GameEngine
{
public:
	SlidingPuzzle()
	{
		SetTitle("Sliding Puzzle");
	}

private:
	Map<int, -1> map;

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

			if (vCellPos < vMapSize && vCellPos > def::vi2d(-1, -1))
			{
				int& nValueOld = map.get(vCellPos.x, vCellPos.y);

				auto check = [&](int ox, int oy)
				{
					int& nValueNew = map.get(vCellPos.x + ox, vCellPos.y + oy);
					if (nValueNew == 0)
					{
						nValueNew = nValueOld;
						nValueOld = 0;

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
