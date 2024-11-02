#define DGE_APPLICATION
#include "../defGameEngine.hpp"

#include <unordered_map>
#include <functional>

using namespace std;

class Mario : public def::GameEngine
{
public:
	Mario()
	{
		SetTitle("Mario");
	}

	~Mario()
	{
		delete sprMario;
		delete sprBackground;
	}

private:
	def::vi2d vTileSize = { 32, 32 };

	enum class TileType
	{
		Empty,
		Coin,
		Grass,
		Dirt
	};

	struct Level
	{
		def::vi2d vSize;
		vector<TileType> vecData;
	};

	static constexpr size_t TILES_SIZE = 4;

	vector<Level> vecLevels;
	size_t nCurrentLevel = 0;

	unordered_map<TileType, def::vi2d> mapSpriteFileOffsets;

	def::vf2d vPlayerPos = { 1.0f, 1.0f };
	def::vf2d vPlayerVel = { 0.0f, 0.0f };

	bool bIsPlayerOnGround = false;

	def::Sprite* sprMario = nullptr;
	def::Sprite* sprBackground = nullptr;

	def::vi2d vGraphicsID;
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
	void LoadLevel(const std::string& sMap, const def::vi2d& vSize, std::unordered_map<char, TileType> mapLookUp)
	{
		CreateLevel(vSize);

		size_t nSaveLevel = nCurrentLevel;
		nCurrentLevel = vecLevels.size() - 1;

		def::vi2d vTile;
		for (vTile.y = 0; vTile.y < vSize.y; vTile.y++)
			for (vTile.x = 0; vTile.x < vSize.x; vTile.x++)
			{
				char c = sMap[vTile.y * vSize.x + vTile.x];
				SetTile(vTile, mapLookUp[c]);
			}

		nCurrentLevel = nSaveLevel;
	}

	void CreateLevel(const def::vi2d& vSize)
	{
		Level level;
		level.vSize = vSize;
		level.vecData.clear();
		level.vecData.resize(level.vSize.x * level.vSize.y);
		vecLevels.push_back(level);
	}

	// Works with vecLevels[nCurrentLevel]
	void SetTile(const def::vi2d& p, TileType tile)
	{
		auto& level = vecLevels[nCurrentLevel];

		if (p.x >= 0 && p.y >= 0 && p.x < level.vSize.x && p.y < level.vSize.y)
			level.vecData[p.y * level.vSize.x + p.x] = tile;
	}

	// Works with vecLevels[nCurrentLevel]
	TileType GetTile(const def::vi2d& p)
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
			if (GetKey(def::Key::LEFT).held)
			{
				vPlayerVel.x += (bIsPlayerOnGround ? -25.0f : -15.0f) * fDeltaTime;

				if (bIsPlayerOnGround)
				{
					SwitchFrame();
					vGraphicsID = { 1, bFrameSwitcher };
				}
			}

			if (GetKey(def::Key::RIGHT).held)
			{
				vPlayerVel.x += (bIsPlayerOnGround ? 25.0f : 15.0f) * fDeltaTime;

				if (bIsPlayerOnGround)
				{
					SwitchFrame();
					vGraphicsID = { 0, bFrameSwitcher };
				}
			}

			if (GetKey(def::Key::SPACE).pressed)
			{
				if (vPlayerVel.y == 0.0f) vPlayerVel.y = -12.0f;
				vGraphicsID = { (vPlayerVel.x >= 0.0f ? 0 : 1), 2 };
			}
		}

		if (bIsPlayerOnGround)
		{
			if (vPlayerVel == def::vf2d())
				vGraphicsID.y = 0;
		}

		fFrameCounter += fDeltaTime;
	}

	void UpdateCollision(float fDeltaTime)
	{
		def::vf2d vNewPlayerPos = vPlayerPos + vPlayerVel * fDeltaTime;

		if (GetTile(vNewPlayerPos) == TileType::Coin)
		{
			SetTile(vNewPlayerPos, TileType::Empty);
			nScore++;
		}

		if (GetTile(vNewPlayerPos + def::vf2d(0.0f, 1.0f)) == TileType::Coin)
		{
			SetTile(vNewPlayerPos + def::vf2d(0.0f, 1.0f), TileType::Empty);
			nScore++;
		}

		if (GetTile(vNewPlayerPos + def::vf2d(1.0f, 0.0f)) == TileType::Coin)
		{
			SetTile(vNewPlayerPos + def::vf2d(1.0f, 0.0f), TileType::Empty);
			nScore++;
		}

		if (GetTile(vNewPlayerPos + def::vf2d(1.0f, 1.0f)) == TileType::Coin)
		{
			SetTile(vNewPlayerPos + def::vf2d(1.0f, 1.0f), TileType::Empty);
			nScore++;
		}

		if (vPlayerVel.x <= 0.0f)
		{
			if (GetTile(def::vi2d(vNewPlayerPos.x + 0.0f, vPlayerPos.y + 0.0f)) != TileType::Empty || GetTile(def::vi2d(vNewPlayerPos.x + 0.0f, vPlayerPos.y + 0.9f)) != TileType::Empty)
			{
				vNewPlayerPos.x = floor(vNewPlayerPos.x) + 1.0f;
				vPlayerVel.x = 0.0f;
			}
		}
		else
		{
			if (GetTile(def::vi2d(vNewPlayerPos.x + 1.0f, vPlayerPos.y + 0.0f)) != TileType::Empty || GetTile(def::vi2d(vNewPlayerPos.x + 1.0f, vPlayerPos.y + 0.9f)) != TileType::Empty)
			{
				vNewPlayerPos.x = floor(vNewPlayerPos.x);
				vPlayerVel.x = 0.0f;
			}
		}

		bIsPlayerOnGround = false;
		if (vPlayerVel.y <= 0.0f)
		{
			if (GetTile(vNewPlayerPos) != TileType::Empty || GetTile(vNewPlayerPos + def::vf2d(0.9f, 0.0f)) != TileType::Empty)
			{
				vNewPlayerPos.y = floor(vNewPlayerPos.y) + 1.0f;
				vPlayerVel.y = 0.0f;
			}
		}
		else
		{
			if (GetTile(vNewPlayerPos + def::vf2d(0.0f, 1.0f)) != TileType::Empty || GetTile(vNewPlayerPos + def::vf2d(0.9f, 1.0f)) != TileType::Empty)
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

		def::vi2d vVisibleTiles = GetScreenSize() / vTileSize;
		def::vf2d vOffset = vPlayerPos - (def::vf2d)vVisibleTiles * 0.5f;

		vOffset = vOffset.max({ 0.0f, 0.0f });
		vOffset = vOffset.min(vecLevels[nCurrentLevel].vSize - vVisibleTiles);

		def::vf2d vTileOffset = (vOffset - vOffset.floor()) * (def::vf2d)vTileSize;

		SetPixelMode(def::Pixel::Mode::ALPHA);

		def::vi2d vTile;
		for (vTile.y = -1; vTile.y < vVisibleTiles.y + 1; vTile.y++)
		{
			for (vTile.x = -1; vTile.x < vVisibleTiles.x + 1; vTile.x++)
			{
				TileType tile = GetTile(vTile + (def::vi2d)vOffset);

				def::vi2d p = def::vf2d(vTile * vTileSize) - vTileOffset;

				if (tile != TileType::Empty)
					DrawPartialSprite(p, mapSpriteFileOffsets[tile], vTileSize, sprMario);
			}
		}

		DrawPartialSprite((vPlayerPos - vOffset) * (def::vf2d)vTileSize, vTileSize * vGraphicsID, vTileSize, sprMario);
	
		SetPixelMode(def::Pixel::Mode::DEFAULT);
	}

	void DrawInterface()
	{
		DrawString(2, 2, "Score: " + to_string(nScore));
	}

	void State_Menu()
	{
		if (GetKey(def::Key::ESCAPE).pressed)
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

		Clear(def::BLUE);

		if (GetKey(def::Key::UP).pressed)
		{
			nMenuCursor--;
			if (nMenuCursor < 0) nMenuCursor = mapOptions.size() - 1;
		}

		if (GetKey(def::Key::DOWN).pressed)
		{
			nMenuCursor++;
			if (nMenuCursor >= mapOptions.size()) nMenuCursor = 0;
		}

		if (GetKey(def::Key::ENTER).pressed)
			vecOptions[nMenuCursor]();

		for (const auto& [sOptionName, nOptionIndex] : mapOptions)
		{
			def::vi2d vOffset = { 10, 20 + 12 * (int)nOptionIndex };

			if (nMenuCursor == nOptionIndex)
			{
				std::string sPointer = "-> ";
				DrawString(vOffset, sPointer);
				vOffset.x += 10 * sPointer.size();
			}

			DrawString(vOffset, sOptionName, def::WHITE);
		}
	}

	void State_Game(float fDeltaTime)
	{
		if (GetKey(def::Key::ESCAPE).pressed)
			nState = GameState::Menu;

		UpdateControls(fDeltaTime);
		ApplyGravity(fDeltaTime);
		UpdateCollision(fDeltaTime);

		DrawWorld();
		DrawInterface();
	}

	virtual bool OnUserCreate() override
	{
		sprMario = new def::Sprite("Assets/mario.png");
		sprBackground = new def::Sprite("Assets/sky.png");

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
	demo.Construct(256, 192, 4, 4);
	demo.Run();
	return 0;
}
