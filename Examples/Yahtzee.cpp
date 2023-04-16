#include "defGameEngine.h"

#include <array>
#include <numeric>
#include <algorithm>
#include <set>

class Example : public def::GameEngine
{
public:
	Example()
	{
		SetTitle("Example");
	}

private:
	std::vector<uint8_t> vecDices;

	void DrawFace(
		const uint8_t nFace,
		const def::vi2d& vPos,
		const def::vi2d& vSize = { 64, 64 },
		const int nRad = 8,
		const def::Pixel& pixDot = def::WHITE,
		const def::Pixel& pixBackground = def::DARK_RED
	)
	{
		def::vi2d vTL = vPos;
		def::vi2d vTR = vPos + def::vi2d(vSize.x, 0);
		def::vi2d vLM = vPos + def::vi2d(0, vSize.y / 2);
		def::vi2d vBL = vPos + def::vi2d(0, vSize.y);
		def::vi2d vBR = vPos + vSize;
		def::vi2d vRM = vPos + def::vi2d(vSize.x, vSize.y / 2);
		def::vi2d vM = vPos + vSize / 2;

		FillRectangle(vPos, vSize, pixBackground);

		if (std::set<uint8_t>{ 2, 3, 4, 5, 6 }.count(nFace) > 0)
		{
			FillCircle(vTL + def::vi2d(nRad + 1, nRad + 1), nRad, pixDot);
			FillCircle(vBR + def::vi2d(-nRad - 2, -nRad - 2), nRad, pixDot);
		}

		if (std::set<uint8_t>{ 4, 5, 6 }.count(nFace) > 0)
		{
			FillCircle(vTR + def::vi2d(-nRad - 2, nRad + 1), nRad, pixDot);
			FillCircle(vBL + def::vi2d(nRad + 1, -nRad - 2), nRad, pixDot);
		}

		if (std::set<uint8_t>{ 6 }.count(nFace) > 0)
		{
			FillCircle(vLM + def::vi2d(nRad + 1, 1), nRad, pixDot);
			FillCircle(vRM + def::vi2d(-nRad - 2, 1), nRad, pixDot);
		}

		if (std::set<uint8_t>{ 1, 3, 5 }.count(nFace) > 0)
			FillCircle(vM, nRad, pixDot);
			
	}

public:
	bool OnUserCreate() override
	{
		srand(time(0));

		vecDices.resize(5);
		for (auto& dice : vecDices) dice = 1;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(def::Key::SPACE).bPressed)
		{
			std::generate(
				vecDices.begin(),
				vecDices.end(),
				[]() { return (rand() % 6) + 1; }
			);

			std::sort(vecDices.begin(), vecDices.end());
		}

		uint8_t nOnes, nTwos, nThrees, nFours, nFives, nSixes, nThreeOfAKind, nFourOfAKind, nFullHouse, nLowStraight, nHighStraight, nYahtzee, nChance;

		nOnes = std::count(vecDices.begin(), vecDices.end(), 1);
		nTwos = std::count(vecDices.begin(), vecDices.end(), 2);
		nThrees = std::count(vecDices.begin(), vecDices.end(), 3);
		nFours = std::count(vecDices.begin(), vecDices.end(), 4);
		nFives = std::count(vecDices.begin(), vecDices.end(), 5);
		nSixes = std::count(vecDices.begin(), vecDices.end(), 6);
		nChance = std::accumulate(vecDices.begin(), vecDices.end(), 0);

		std::vector<uint8_t> vecValues =
		{
			nOnes,
			nTwos,
			nThrees,
			nFours,
			nFives,
			nSixes
		};

		auto Match = [&](const std::vector<uint8_t>& v, uint8_t kind)
		{
			bool bResult = false;

			for (const auto& n : v)
				bResult |= (n == kind);

			return bResult;
		};

		nThreeOfAKind = Match(vecValues, 3) ? nChance : 0;
		nFourOfAKind = Match(vecValues, 4) ? nChance : 0;
		nFullHouse = Match(vecValues, 3) && Match(vecValues, 2) ? nChance : 0;

		auto LongestSequence = [](const std::vector<uint8_t>& v)
		{
			size_t nCurrentSequence = 1;
			size_t nMaxSequence = 1;
			
			for (size_t i = 1; i < v.size(); i++)
			{
				if (v[i - 1] == (v[i] - 1))
				{
					nCurrentSequence++;
				}
				else
				{
					if (v[i] != v[i - 1])
						nCurrentSequence = 1;
				}

				nMaxSequence = std::max(nMaxSequence, nCurrentSequence);
			}

			return nMaxSequence;
		};

		nLowStraight = (LongestSequence(vecDices) == 4) ? 30 : 0;
		nHighStraight = (LongestSequence(vecDices) == 5) ? 40 : 0;
		nYahtzee = (Match(vecValues, 5)) ? 50 : 0;

		Clear(def::DARK_GREEN);

		def::vi2d vPos = { -58, 10 };
		for (const auto& face : vecDices) DrawFace(face, vPos += def::vi2d(68, 0));

		vPos = { 10, 64 };

		std::vector<std::pair<std::string, int>> vecScoreTable =
		{
			{ "Ones         ", nOnes },
			{ "Twos         ", nTwos },
			{ "Threes       ", nThrees },
			{ "Fours        ", nFours },
			{ "Fives        ", nFives },
			{ "Sixes        ", nSixes },
			{ "3-of-a-Kind  ", nThreeOfAKind },
			{ "4-of-a-Kind  ", nFourOfAKind },
			{ "Full House   ", nFullHouse },
			{ "Low Straight ", nLowStraight },
			{ "High Straight", nHighStraight },
			{ "Yahtzee      ", nYahtzee },
			{ "Chance       ", nChance }
		};

		def::vi2d vOffset = { 0, 0 };
		for (const auto& score : vecScoreTable) DrawString(vPos + (vOffset += def::vi2d(0, 16)), score.first + " : " + std::to_string(score.second));

		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(400, 300, 2, 2))
		demo.Run();
	return 0;
}
