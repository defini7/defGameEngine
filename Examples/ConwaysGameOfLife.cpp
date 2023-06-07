#define DGE_APPLICATION
#include "defGameEngine.h"

#include <algorithm>

using namespace std;

class CellularAutomata : public def::GameEngine
{
public:
	CellularAutomata()
	{
		SetTitle("Cellular Automata");
	}

private:
	bool* output = nullptr;
	bool* state = nullptr;

	float fTicks = 0.0f;
	float fDelay = 0.0f;
	
	std::vector<std::vector<int>> vStateLookup;

private:
	int GetNeighboursCount(int x, int y)
	{
		int nCount = 0;

		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
				nCount += output[(y + j) * ScreenWidth() + (x + i)];
		}

		if (output[y * ScreenWidth() + x]) nCount--;

		return nCount;
	}

protected:
	bool OnUserCreate() override
	{
		output = new bool[ScreenWidth() * ScreenHeight()];
		state = new bool[ScreenWidth() * ScreenHeight()];

		memset(output, 0, ScreenWidth() * ScreenHeight() * sizeof(bool));
		memset(state, 0, ScreenWidth() * ScreenHeight() * sizeof(bool));

		/*for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
			state[i] = (bool)(rand() % 2);*/

		auto set = [&](int x, int y, std::string v)
		{
			for (int i = 0; i < v.size(); i++)
				state[y * ScreenWidth() + (x + i)] = (v[i] == '#');
		};

		set(60, 45, "........................#............");
		set(60, 46, "......................#.#............");
		set(60, 47, "............##......##............##.");
		set(60, 48, "...........#...#....##............##.");
		set(60, 49, "##........#.....#...##...............");
		set(60, 50, "##........#...#.##....#.#............");
		set(60, 51, "..........#.....#.......#............");
		set(60, 52, "...........#...#.....................");
		set(60, 53, "............##.......................");

		vStateLookup =
		{
			{ 0, 0, 0, 1, 0, 0, 0, 0, 0 }, // dead
			{ 0, 0, 1, 1, 0, 0, 0, 0, 0 }  // alive
		};

		DrawRectangle(0, 0, ScreenWidth() - 1, ScreenHeight() - 1, def::RED);
		UpdateState();

		return true;
	}

	void UpdateState()
	{
		for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
			output[i] = state[i];

		for (int x = 1; x < ScreenWidth() - 1; x++)
			for (int y = 1; y < ScreenHeight() - 1; y++)
			{
				int nCount = GetNeighboursCount(x, y);

				size_t i = y * ScreenWidth() + x;
				state[i] = (bool)vStateLookup[output[i]][nCount];
			}
	}

	void UpdateBoth(int x, int y, bool v)
	{
		size_t i = y * ScreenWidth() + x;
		state[i] = output[i] = v;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (GetKey(def::Key::SPACE).bPressed)
			UpdateState();

		if (GetKey(def::Key::LEFT_SHIFT).bHeld)
		{
			if (fTicks >= fDelay)
			{
				UpdateState();
				fTicks = 0.0f;
			}
			
			fTicks += fDeltaTime;
		}

		if (GetKey(def::Key::LEFT).bHeld) fDelay -= fDeltaTime;
		if (GetKey(def::Key::RIGHT).bHeld) fDelay += fDeltaTime;

		fDelay = std::clamp(fDelay, 0.0f, 1.0f);

		if (GetMouse(0).bHeld) UpdateBoth(MouseX(), MouseY(), true);
		if (GetMouse(1).bHeld) UpdateBoth(MouseX(), MouseY(), false);

		for (int x = 1; x < ScreenWidth() - 1; x++)
			for (int y = 1; y < ScreenHeight() - 1; y++)
				Draw(x, y, output[y * ScreenWidth() + x] ? def::WHITE : def::BLACK);

		DrawString(2, 2, "Delay = " + std::to_string(fDelay), def::YELLOW);

		return true;
	}
};

int main()
{
	srand(clock());

	CellularAutomata game;

	game.Construct(160, 100, 8, 8);
	game.Run();

	return 0;
}
