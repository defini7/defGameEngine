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

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <unordered_map>

using namespace std;

class Mario : public olc::PixelGameEngine
{
public:
	Mario()
	{
		sAppName = "Mario";
	}

	~Mario()
	{
		delete sprMario;
		delete sprBackground;
	}

private:
	olc::vi2d vTileSize = { 32, 32 };

	enum class TileType
	{
		Empty,
		Coin,
		Grass,
		Dirt
	};

	struct Level
	{
		olc::vi2d vSize;
		vector<TileType> vecData;
	};

	static constexpr size_t TILES_SIZE = 4;

	vector<Level> vecLevels;
	size_t nCurrentLevel = 0;

	unordered_map<TileType, olc::vi2d> mapSpriteFileOffsets;

	olc::vf2d vPlayerPos = { 1.0f, 1.0f };
	olc::vf2d vPlayerVel = { 0.0f, 0.0f };

	bool bIsPlayerOnGround = false;

	olc::Sprite* sprMario = nullptr;
	olc::Sprite* sprBackground = nullptr;

	olc::vi2d vGraphicsID;
	int nScore = 0;

	enum GameState
	{
		Menu,
		Game
	} nState = Menu;

	int nMenuCursor = 0;

	bool bRunning = true;

	float fFrameCounter = 0.0f;
	bool bFrameSwitcher = 0;

protected:
	// That doesn't modify nCurrentLevel, just pushes a new level at the end of the vector
	void LoadLevel(const std::string& sMap, const olc::vi2d& vSize, std::unordered_map<char, TileType> mapLookUp)
	{
		CreateLevel(vSize);

		size_t nSaveLevel = nCurrentLevel;
		nCurrentLevel = vecLevels.size() - 1;

		olc::vi2d vTile;
		for (vTile.y = 0; vTile.y < vSize.y; vTile.y++)
			for (vTile.x = 0; vTile.x < vSize.x; vTile.x++)
			{
				char c = sMap[vTile.y * vSize.x + vTile.x];
				SetTile(vTile, mapLookUp[c]);
			}

		nCurrentLevel = nSaveLevel;
	}

	void CreateLevel(const olc::vi2d& vSize)
	{
		Level level;
		level.vSize = vSize;
		level.vecData.clear();
		level.vecData.resize(level.vSize.x * level.vSize.y);
		vecLevels.push_back(level);
	}

	// Works with vecLevels[nCurrentLevel]
	void SetTile(const olc::vi2d& p, TileType tile)
	{
		auto& level = vecLevels[nCurrentLevel];

		if (p.x >= 0 && p.y >= 0 && p.x < level.vSize.x && p.y < level.vSize.y)
			level.vecData[p.y * level.vSize.x + p.x] = tile;
	}

	// Works with vecLevels[nCurrentLevel]
	TileType GetTile(const olc::vi2d& p)
	{
		auto& level = vecLevels[nCurrentLevel];

		if (p.x >= 0 && p.y >= 0 && p.x < level.vSize.x && p.y < level.vSize.y)
			return level.vecData[p.y * level.vSize.x + p.x];

		return TileType::Empty;
	}

	void SwitchFrame(float fPeriod = 0.2f)
	{
		if (fFrameCounter >= fPeriod)
		{
			bFrameSwitcher = !bFrameSwitcher;
			fFrameCounter = 0.0f;
		}
	}

	void UpdateControls(float fDeltaTime)
	{
		if (IsFocused())
		{
			if (GetKey(olc::Key::LEFT).bHeld)
			{
				vPlayerVel.x += (bIsPlayerOnGround ? -25.0f : -15.0f) * fDeltaTime;

				if (bIsPlayerOnGround)
				{
					SwitchFrame();
					vGraphicsID = { 1, bFrameSwitcher };
				}
			}

			if (GetKey(olc::Key::RIGHT).bHeld)
			{
				vPlayerVel.x += (bIsPlayerOnGround ? 25.0f : 15.0f) * fDeltaTime;

				if (bIsPlayerOnGround)
				{
					SwitchFrame();
					vGraphicsID = { 0, bFrameSwitcher };
				}
			}

			if (GetKey(olc::Key::SPACE).bPressed)
			{
				if (vPlayerVel.y == 0.0f) vPlayerVel.y = -12.0f;
				vGraphicsID = { (vPlayerVel.x >= 0.0f ? 0 : 1), 2 };
			}
		}

		fFrameCounter += fDeltaTime;
	}

	void UpdateCollision(float fDeltaTime)
	{
		olc::vf2d vNewPlayerPos = vPlayerPos + vPlayerVel * fDeltaTime;

		if (GetTile(vNewPlayerPos) == TileType::Coin)
		{
			SetTile(vNewPlayerPos, TileType::Empty);
			nScore++;
		}

		if (GetTile(vNewPlayerPos + olc::vf2d(0.0f, 1.0f)) == TileType::Coin)
		{
			SetTile(vNewPlayerPos + olc::vf2d(0.0f, 1.0f), TileType::Empty);
			nScore++;
		}

		if (GetTile(vNewPlayerPos + olc::vf2d(1.0f, 0.0f)) == TileType::Coin)
		{
			SetTile(vNewPlayerPos + olc::vf2d(1.0f, 0.0f), TileType::Empty);
			nScore++;
		}

		if (GetTile(vNewPlayerPos + olc::vf2d(1.0f, 1.0f)) == TileType::Coin)
		{
			SetTile(vNewPlayerPos + olc::vf2d(1.0f, 1.0f), TileType::Empty);
			nScore++;
		}

		if (vPlayerVel.x <= 0.0f)
		{
			if (GetTile(olc::vi2d(vNewPlayerPos.x + 0.0f, vPlayerPos.y + 0.0f)) != TileType::Empty || GetTile(olc::vi2d(vNewPlayerPos.x + 0.0f, vPlayerPos.y + 0.9f)) != TileType::Empty)
			{
				vNewPlayerPos.x = floor(vNewPlayerPos.x) + 1.0f;
				vPlayerVel.x = 0.0f;
			}
		}
		else
		{
			if (GetTile(olc::vi2d(vNewPlayerPos.x + 1.0f, vPlayerPos.y + 0.0f)) != TileType::Empty || GetTile(olc::vi2d(vNewPlayerPos.x + 1.0f, vPlayerPos.y + 0.9f)) != TileType::Empty)
			{
				vNewPlayerPos.x = floor(vNewPlayerPos.x);
				vPlayerVel.x = 0.0f;
			}
		}

		bIsPlayerOnGround = false;
		if (vPlayerVel.y <= 0.0f)
		{
			if (GetTile(vNewPlayerPos) != TileType::Empty || GetTile(vNewPlayerPos + olc::vf2d(0.9f, 0.0f)) != TileType::Empty)
			{
				vNewPlayerPos.y = floor(vNewPlayerPos.y) + 1.0f;
				vPlayerVel.y = 0.0f;
			}
		}
		else
		{
			if (GetTile(vNewPlayerPos + olc::vf2d(0.0f, 1.0f)) != TileType::Empty || GetTile(vNewPlayerPos + olc::vf2d(0.9f, 1.0f)) != TileType::Empty)
			{
				vNewPlayerPos.y = floor(vNewPlayerPos.y);
				vPlayerVel.y = 0.0f;
				bIsPlayerOnGround = true;
			}
		}

		auto& level = vecLevels[nCurrentLevel];

		if (vNewPlayerPos.x < 0.0f)
		{
			if ((int)nCurrentLevel - 1 >= 0)
			{
				nCurrentLevel--;
				vNewPlayerPos.x += (float)level.vSize.x - 1.0f;
			}
			else
			{
				vNewPlayerPos.x = floor(vNewPlayerPos.x) + 1.0f;
				vPlayerVel.x = 0.0f;
			}
		}
		else if (vNewPlayerPos.x >= (float)level.vSize.x - 1.0f)
		{
			if (nCurrentLevel + 1 < vecLevels.size())
			{
				nCurrentLevel++;
				vNewPlayerPos.x -= (float)level.vSize.x - 1.0f;
			}
			else
			{
				vNewPlayerPos.x = floor(vNewPlayerPos.x);
				vPlayerVel.x = 0.0f;
			}
		}

		vPlayerPos = vNewPlayerPos;
	}

	void ApplyGravity(float fDeltaTime)
	{
		vPlayerVel.y += 20.0f * fDeltaTime;

		if (bIsPlayerOnGround)
		{
			vPlayerVel.x += -3.0f * vPlayerVel.x * fDeltaTime;
			if (fabs(vPlayerVel.x) < 0.01f) vPlayerVel.x = 0.0f;
		}

		if (vPlayerVel.x > 10.0f) vPlayerVel.x = 10.0f;
		if (vPlayerVel.x < -10.0f) vPlayerVel.x = -10.0f;
		if (vPlayerVel.y > 100.0f) vPlayerVel.y = 100.0f;
		if (vPlayerVel.y < -100.0f) vPlayerVel.y = -100.0f;
	}

	void DrawWorld()
	{
		DrawSprite(0, 0, sprBackground);

		olc::vi2d vVisibleTiles = GetScreenSize() / vTileSize;
		olc::vf2d vOffset = vPlayerPos - (olc::vf2d)vVisibleTiles * 0.5f;

		vOffset = vOffset.max({ 0.0f, 0.0f });
		vOffset = vOffset.min(vecLevels[nCurrentLevel].vSize - vVisibleTiles);

		olc::vf2d vTileOffset = (vOffset - vOffset.floor()) * (olc::vf2d)vTileSize;

		SetPixelMode(olc::Pixel::ALPHA);

		olc::vi2d vTile;
		for (vTile.y = -1; vTile.y < vVisibleTiles.y + 1; vTile.y++)
		{
			for (vTile.x = -1; vTile.x < vVisibleTiles.x + 1; vTile.x++)
			{
				TileType tile = GetTile(vTile + (olc::vi2d)vOffset);

				olc::vi2d p = olc::vf2d(vTile * vTileSize) - vTileOffset;

				if (tile != TileType::Empty)
					DrawPartialSprite(p, sprMario, mapSpriteFileOffsets[tile], vTileSize);
			}
		}

		DrawPartialSprite((vPlayerPos - vOffset) * (olc::vf2d)vTileSize, sprMario, vTileSize * vGraphicsID, vTileSize);
	
		SetPixelMode(olc::Pixel::NORMAL);
	}

	void DrawInterface()
	{
		DrawString(2, 2, "Score: " + to_string(nScore));
	}

	void State_Menu()
	{
		if (GetKey(olc::Key::ESCAPE).bPressed)
		{
			nMenuCursor = 0;
			nState = GameState::Game;
		}

		std::unordered_map<std::string, size_t> mapOptions =
		{
			{ "Continue", 0 },
			{ "Credits", 1 },
			{ "Exit", 2 }
		};

		std::vector<std::function<void()>> vecOptions =
		{
			[&]()
			{
				nMenuCursor = 0;
				nState = GameState::Game;
			},

			[&]() {},
			[&]() { bRunning = false; }
		};

		Clear(olc::BLUE);

		if (GetKey(olc::Key::UP).bPressed)
		{
			nMenuCursor--;
			if (nMenuCursor < 0) nMenuCursor = mapOptions.size() - 1;
		}

		if (GetKey(olc::Key::DOWN).bPressed)
		{
			nMenuCursor++;
			if (nMenuCursor >= mapOptions.size()) nMenuCursor = 0;
		}

		if (GetKey(olc::Key::ENTER).bPressed)
			vecOptions[nMenuCursor]();

		for (const auto& [sOptionName, nOptionIndex] : mapOptions)
		{
			olc::vi2d vOffset = { 10, 20 + (GetTextSize(sOptionName).y + 2) * (int)nOptionIndex };

			if (nMenuCursor == nOptionIndex)
			{
				std::string sPointer = "-> ";
				DrawString(vOffset, sPointer);
				vOffset.x += GetTextSize(sPointer).x;
			}

			DrawString(vOffset, sOptionName, olc::WHITE);
		}
	}

	void State_Game(float fDeltaTime)
	{
		if (GetKey(olc::Key::ESCAPE).bPressed)
			nState = GameState::Menu;

		UpdateControls(fDeltaTime);
		ApplyGravity(fDeltaTime);
		UpdateCollision(fDeltaTime);

		DrawWorld();
		DrawInterface();
	}

	virtual bool OnUserCreate() override
	{
		sprMario = new olc::Sprite("Assets/mario.png");
		sprBackground = new olc::Sprite("Assets/sky.png");

		mapSpriteFileOffsets =
		{
			{ TileType::Coin, { 2 * vTileSize.x, 0 } },
			{ TileType::Grass, { 3 * vTileSize.x, 0} },
			{ TileType::Dirt, { 4 * vTileSize.x, 0} }
		};

		std::unordered_map<char, TileType> mapLookUp =
		{
			{ '.', TileType::Empty },
			{ '#', TileType::Dirt },
			{ 'x', TileType::Grass },
			{ 'o', TileType::Coin }
		};

		std::string sMap;
		sMap  = "........xxxxxxxx";
		sMap += ".....ooo........";
		sMap += "................";
		sMap += "..xxxxxxxx...xxx";
		sMap += "..########...###";
		sMap += "..########..x###";
		sMap += "................";
		sMap += "xxxxxxxxxxxxxxxx";

		LoadLevel(sMap, { 16, 8 }, mapLookUp);

		sMap  = ".xx.....x.......";
		sMap += ".....ooo........";
		sMap += "..............o.";
		sMap += ".....xxxxx...xxx";
		sMap += "......###....###";
		sMap += "..xxx.......,...";
		sMap += "................";
		sMap += "xxxxxxxxxxxxxxxx";

		LoadLevel(sMap, { 16, 8 }, mapLookUp);

		return true;
	}

	virtual bool OnUserUpdate(float fDeltaTime) override
	{
		switch (nState)
		{
		case GameState::Menu: State_Menu(); break;
		case GameState::Game: State_Game(fDeltaTime); break;
		}

		return bRunning;
	}
};

int main()
{
	Mario demo;
	if (demo.Construct(256, 192, 4, 4))
		demo.Start();
	return 0;
}
