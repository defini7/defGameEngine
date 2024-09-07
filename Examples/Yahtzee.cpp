/*
* BSD 3-Clause License
Copyright (c) 2022 - 2024, Alex
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
#include "defGameEngine.hpp"

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
	std::vector<uint8_t> dices;

	void DrawFace(
		const uint8_t face,
		const def::vi2d& pos,
		const def::vi2d& size = { 64, 64 },
		const int radius = 8,
		const def::Pixel& dotCol = def::WHITE,
		const def::Pixel& backgroundCol = def::DARK_RED
	)
	{
		def::vi2d vTL = pos;
		def::vi2d vTR = pos + def::vi2d(size.x, 0);
		def::vi2d vLM = pos + def::vi2d(0, size.y / 2);
		def::vi2d vBL = pos + def::vi2d(0, size.y);
		def::vi2d vBR = pos + size;
		def::vi2d vRM = pos + def::vi2d(size.x, size.y / 2);
		def::vi2d vM = pos + size / 2;

		FillRectangle(pos, size, backgroundCol);

		if (std::set<uint8_t>{ 2, 3, 4, 5, 6 }.count(face) > 0)
		{
			FillCircle(vTL + def::vi2d(radius + 1, radius + 1), radius, dotCol);
			FillCircle(vBR + def::vi2d(-radius - 2, -radius - 2), radius, dotCol);
		}

		if (std::set<uint8_t>{ 4, 5, 6 }.count(face) > 0)
		{
			FillCircle(vTR + def::vi2d(-radius - 2, radius + 1), radius, dotCol);
			FillCircle(vBL + def::vi2d(radius + 1, -radius - 2), radius, dotCol);
		}

		if (std::set<uint8_t>{ 6 }.count(face) > 0)
		{
			FillCircle(vLM + def::vi2d(radius + 1, 1), radius, dotCol);
			FillCircle(vRM + def::vi2d(-radius - 2, 1), radius, dotCol);
		}

		if (std::set<uint8_t>{ 1, 3, 5 }.count(face) > 0)
			FillCircle(vM, radius, dotCol);

	}

public:
	bool OnUserCreate() override
	{
		srand(time(0));

		dices.resize(5);
		for (auto& dice : dices) dice = 1;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(def::Key::SPACE).pressed)
		{
			std::generate(
				dices.begin(),
				dices.end(),
				[]() { return (rand() % 6) + 1; }
			);

			std::sort(dices.begin(), dices.end());
		}

		uint8_t ones, twos, threes, fours, fives, sixes, threeOfAKind, fourOfAKind, fullHouse, lowStraight, highStraight, yahtzee, chance;

		ones = std::count(dices.begin(), dices.end(), 1);
		twos = std::count(dices.begin(), dices.end(), 2);
		threes = std::count(dices.begin(), dices.end(), 3);
		fours = std::count(dices.begin(), dices.end(), 4);
		fives = std::count(dices.begin(), dices.end(), 5);
		sixes = std::count(dices.begin(), dices.end(), 6);
		chance = std::accumulate(dices.begin(), dices.end(), 0);

		std::vector<uint8_t> vecValues =
		{
			ones,
			twos,
			threes,
			fours,
			fives,
			sixes
		};

		auto Match = [&](const std::vector<uint8_t>& v, uint8_t kind)
		{
			bool result = false;
			for (const auto& n : v) result |= (n == kind);
			return result;
		};

		threeOfAKind = Match(vecValues, 3) ? chance : 0;
		fourOfAKind = Match(vecValues, 4) ? chance : 0;
		fullHouse = Match(vecValues, 3) && Match(vecValues, 2) ? chance : 0;

		auto LongestSequence = [](const std::vector<uint8_t>& v)
		{
			size_t currentSequence = 1;
			size_t maxSequence = 1;

			for (size_t i = 1; i < v.size(); i++)
			{
				if (v[i - 1] == (v[i] - 1))
				{
					currentSequence++;
				}
				else
				{
					if (v[i] != v[i - 1])
						currentSequence = 1;
				}

				maxSequence = std::max(maxSequence, currentSequence);
			}

			return maxSequence;
		};

		lowStraight = (LongestSequence(dices) == 4) ? 30 : 0;
		highStraight = (LongestSequence(dices) == 5) ? 40 : 0;
		yahtzee = (Match(vecValues, 5)) ? 50 : 0;

		Clear(def::DARK_GREEN);

		def::vi2d pos = { -58, 10 };
		for (const auto& face : dices) DrawFace(face, pos += def::vi2d(68, 0));

		pos = { 10, 64 };

		std::vector<std::pair<std::string, int>> vecScoreTable =
		{
			{ "Ones         ", ones },
			{ "Twos         ", twos },
			{ "Threes       ", threes },
			{ "Fours        ", fours },
			{ "Fives        ", fives },
			{ "Sixes        ", sixes },
			{ "3-of-a-Kind  ", threeOfAKind },
			{ "4-of-a-Kind  ", fourOfAKind },
			{ "Full House   ", fullHouse },
			{ "Low Straight ", lowStraight },
			{ "High Straight", highStraight },
			{ "Yahtzee      ", yahtzee },
			{ "Chance       ", chance }
		};

		def::vi2d offset = { 0, 0 };
		for (const auto& score : vecScoreTable) DrawString(pos + (offset += def::vi2d(0, 16)), score.first + " : " + std::to_string(score.second));

		return true;
	}
};

int main()
{
	Example demo;
	demo.Construct(400, 300, 2, 2);
	demo.Run();
	return 0;
}
