#include "defGameEngine.h"

#include <array>
#include <numeric>
#include <algorithm>

#define X(i, n) sPatterns[i] = n

class Example : public def::GameEngine
{
public:
	Example()
	{
		SetTitle("Example");
		ShowFPS();
	}

private:
	std::array<std::string, 6> sPatterns;

	std::vector<int> vecDices;

public:
	bool OnUserCreate() override
	{
		srand(time(0));

		X(0, "ooo"
			"oxo"
			"ooo"
		);

		X(1, "oox"
			"ooo"
			"xoo"
		);

		X(2, "oox"
			"oxo"
			"xoo"
		);

		X(3, "xox"
			"ooo"
			"xox"
		);

		X(4, "xox"
			"oxo"
			"xox"
		);

		X(5, "xox"
			"xox"
			"xox"
		);

		vecDices.resize(5);
		for (auto& dice : vecDices) dice = 1;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(def::Key::SPACE).bPressed)
		{
			auto Roll = [&sPatterns = sPatterns](int& dice)
			{
				int nNewPattern = dice;

				do
				{
					nNewPattern = (rand() % sPatterns.size()) + 1;
				} while (nNewPattern == dice);

				dice = nNewPattern;
			};

			for (auto& dice : vecDices) Roll(dice);
		}

		std::vector<std::pair<std::string, int>> vecScores =
		{
			{ "Ones", std::count(vecDices.begin(), vecDices.end(), 1) },
			{ "Twos", std::count(vecDices.begin(), vecDices.end(), 2) },
			{ "Threes", std::count(vecDices.begin(), vecDices.end(), 3) },
			{ "Fours", std::count(vecDices.begin(), vecDices.end(), 4) },
			{ "Fives", std::count(vecDices.begin(), vecDices.end(), 5) },
			{ "Sixes", std::count(vecDices.begin(), vecDices.end(), 6) },
			{ "3-of-a-Kind", 0 },
			{ "4-of-a-Kind", 0 },
			{ "Full House", 0 },
			{ "Low Straight", 0 },
			{ "High Straight", 0 },
			{ "Yahtzee", 0 },
			{ "Chance", 0 }
		};

		// Chance
		vecScores.back().second = std::accumulate(vecDices.begin(), vecDices.end(), 0);

		std::vector<int> vecValues;
		for (size_t i = 0; i < 6; i++) vecValues.push_back(vecScores[i].second);

		auto Match = [&](const std::vector<int>& v, int kind)
		{
			bool bResult = false;

			for (const auto& n : v)
				bResult |= (n == kind);

			return bResult;
		};

		// 3-of-a-Kind, 4-of-a-Kind
		for (int i = 0; i < 2; i++)
		{
			if (Match(vecValues, 3 + i))
				vecScores[6 + i].second = vecScores.back().second;
		}

		// Full House
		if (Match(vecValues, 3) && Match(vecValues, 2))
			vecScores[8].second = vecScores.back().second;



		auto LongestSequence = [](const std::vector<int>& v)
		{
			std::vector<int> vecSorted(v);
			std::sort(vecSorted.begin(), vecSorted.end());

			size_t nCurrentSeq = 1;
			size_t nMaxSeq = 1;

			for (int i = 1; i < vecSorted.size(); i++)
			{
				if (vecSorted[i - 1] == (vecSorted[i] - 1))
					nCurrentSeq++;
				else
				{
					if (vecSorted[i] != vecSorted[i - 1])
						nCurrentSeq = 1;
				}

				nMaxSeq = std::max(nMaxSeq, nCurrentSeq);
			}

			return nMaxSeq;
		};

		// Low Straight
		if (LongestSequence(vecDices) == 4)
			vecScores[9].second = 30;

		// High Straight
		if (LongestSequence(vecDices) == 5)
			vecScores[10].second = 40;

		// Yahtzee
		if (Match(vecValues, 5))
			vecScores[11].second = 50;

		Clear(def::DARK_GREEN);

		def::vi2d vPos = { 1, 1 };
		for (auto& dice : vecDices)
		{
			int i = 0;
			for (const auto& c : sPatterns[dice - 1])
			{
				def::vi2d pos = def::vi2d(i % 3, i / 3);
				def::vi2d size = { 9, 9 };

				char c = sPatterns[dice - 1][pos.y * 3 + pos.x];

				pos += vPos;
				FillRectangle(pos * size, size, def::DARK_RED);

				if (c == 'x')
					FillCircle(pos * size + size / 2, size.x / 2 - 1, def::WHITE);

				i++;
			}

			vPos.x += 4;
		}

		vPos += def::vi2d(-8, 32);

		def::vi2d vOffset = { 0, 0 };
		for (const auto& score : vecScores) DrawString(vPos + (vOffset += def::vi2d(0, 16)), score.first + ": " + std::to_string(score.second));

		return true;
	}
};

int main()
{
	Example demo;
  
	if (demo.Construct(256, 240, 4, 4))
		demo.Run();
  
	return 0;
}
