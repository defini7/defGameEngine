#define DGE_APPLICATION
#include "defGameEngine.h"

#include <unordered_map>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#ifdef _WIN32
#pragma comment(lib, "liblua54.a")
#endif

using namespace std;
using namespace def;

bool CheckLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		return false;
	}
	return true;
}

class Mario : public def::GameEngine
{
public:
	Mario()
	{
		SetTitle("Mario");
		ShowFPS();
	}

	~Mario()
	{
		lua_close(script);

		delete sprMario;
		delete sprBackground;
	}

private:
	lua_State* script;

	vi2d vLevelSize = { 16, 8 };
	vi2d vTileSize = { 32, 32 };

	enum class TileType
	{
		Empty,
		Coin,
		Grass,
		Dirt
	};

	static constexpr size_t TILES_SIZE = 4;

	vector<TileType> vecLevel;

	unordered_map<TileType, vi2d> mapSpriteFileOffsets;

	vf2d vPlayerPos = { 1.0f, 1.0f };
	vf2d vPlayerVel = { 0.0f, 0.0f };

	bool bPlayerOnGround = false;

	def::Sprite* sprMario = nullptr;
	def::Sprite* sprBackground = nullptr;

	int nGraphicID = 0;
	int nScore = 0;

protected:
	void CreateLevel(const vi2d& levelSize)
	{
		vLevelSize = levelSize;
		vecLevel.clear();
		vecLevel.resize(vLevelSize.x * vLevelSize.y);
	}

	static int wrap_CreateLevel(lua_State* L)
	{
		if (lua_gettop(L) != 3) return -1;

		Mario* obj = static_cast<Mario*>(lua_touserdata(L, 1));
		vi2d mapSize = vi2d(lua_tointeger(L, 2), lua_tointeger(L, 3));

		obj->CreateLevel(mapSize);

		return 0;
	}

	void SetTile(const vi2d& p, TileType tile)
	{
		if (p >= vi2d(0, 0) && p < vLevelSize)
			vecLevel[p.y * vLevelSize.x + p.x] = tile;
	}

	static int wrap_SetTile(lua_State* L)
	{
		if (lua_gettop(L) != 4) return -1;

		Mario* obj = static_cast<Mario*>(lua_touserdata(L, 1));
		vi2d mapPos = vi2d(lua_tointeger(L, 2), lua_tointeger(L, 3));
		int tile = lua_tointeger(L, 4);

		obj->SetTile(mapPos, (TileType)tile);

		return 0;
	}

	TileType GetTile(const vi2d& p)
	{
		if (p >= vi2d(0, 0) && p < vLevelSize)
			return vecLevel[p.y * vLevelSize.x + p.x];

		return TileType::Empty;
	}

	static int wrap_GetTile(lua_State* L)
	{
		if (lua_gettop(L) != 3) return -1;

		Mario* obj = static_cast<Mario*>(lua_touserdata(L, 1));
		vi2d mapPos = vi2d(lua_tointeger(L, 2), lua_tointeger(L, 3));

		int tile = (int)obj->GetTile(mapPos);
		lua_pushinteger(L, tile);

		return 1;
	}

	virtual bool OnUserCreate() override
	{
		script = luaL_newstate();
		luaL_openlibs(script);

		lua_register(script, "CreateLevel", wrap_CreateLevel);
		lua_register(script, "SetTile", wrap_SetTile);
		lua_register(script, "GetTile", wrap_GetTile);

		if (CheckLua(script, luaL_dofile(script, "main.lua")))
		{
			lua_getglobal(script, "LoadLevel");
			if (lua_isfunction(script, -1))
			{
				lua_pushlightuserdata(script, this);
				lua_pushnumber(script, 1);

				if (!CheckLua(script, lua_pcall(script, 2, 1, 0)))
					return false;
			}

			for (int tile = 0; tile < TILES_SIZE; tile++)
			{
				lua_getglobal(script, "LoadSprite");
				if (lua_isfunction(script, -1))
				{
					lua_pushlightuserdata(script, this);
					lua_pushnumber(script, tile);

					if (!CheckLua(script, lua_pcall(script, 2, 1, 0)))
						return false;

					if (lua_istable(script, -1))
					{
						vi2d filePos;

						lua_getfield(script, -1, "x");
						filePos.x = lua_tonumber(script, -1);

						lua_pop(script, 1);

						lua_getfield(script, -1, "y");
						filePos.y = lua_tonumber(script, -1);

						lua_pop(script, 1);

						mapSpriteFileOffsets[(TileType)tile] = filePos;
					}
				}
			}
		}

		lua_getglobal(script, "SPRITES_FILE");
		sprMario = new def::Sprite(lua_tostring(script, -1));

		lua_getglobal(script, "BACKGROUND_FILE");
		sprBackground = new def::Sprite(lua_tostring(script, -1));

		lua_getglobal(script, "TILE_SIZE");
		if (lua_istable(script, -1))
		{
			lua_getfield(script, -1, "x");
			vTileSize.x = lua_tonumber(script, -1);

			lua_pop(script, 1);

			lua_getfield(script, -1, "y");
			vTileSize.y = lua_tonumber(script, -1);

			lua_pop(script, 1);
		}

		return true;
	}

	virtual bool OnUserUpdate(float fDeltaTime) override
	{
		if (GetWindowState() & def::WS_FOCUSED)
		{
			if (GetKey(def::Key::UP).bHeld)
			{
				vPlayerVel.y = -6.0f;
				nGraphicID = (vPlayerVel.x >= 0.0f) ? 2 : 3;
			}

			if (GetKey(def::Key::DOWN).bHeld)
				vPlayerVel.y = 6.0f;

			if (GetKey(def::Key::LEFT).bHeld)
			{
				vPlayerVel.x += (bPlayerOnGround ? -25.0f : -15.0f) * fDeltaTime;
				if (bPlayerOnGround) nGraphicID = 1;
			}

			if (GetKey(def::Key::RIGHT).bHeld)
			{
				vPlayerVel.x += (bPlayerOnGround ? 25.0f : 15.0f) * fDeltaTime;
				if (bPlayerOnGround) nGraphicID = 0;
			}

			if (GetKey(def::Key::SPACE).bPressed)
			{
				if (vPlayerVel.y == 0.0f)
					vPlayerVel.y = -12.0f;

				nGraphicID = (vPlayerVel.x >= 0.0f) ? 2 : 3;
			}
		}

		vPlayerVel.y += 20.0f * fDeltaTime;

		if (bPlayerOnGround)
		{
			vPlayerVel.x += -3.0f * vPlayerVel.x * fDeltaTime;
			if (fabs(vPlayerVel.x) < 0.01f) vPlayerVel.x = 0.0f;
		}

		if (vPlayerVel.x > 10.0f)
			vPlayerVel.x = 10.0f;

		if (vPlayerVel.x < -10.0f)
			vPlayerVel.x = -10.0f;

		if (vPlayerVel.y > 100.0f)
			vPlayerVel.y = 100.0f;

		if (vPlayerVel.y < -100.0f)
			vPlayerVel.y = -100.0f;

		vf2d vNewPlayerPos = vPlayerPos + vPlayerVel * fDeltaTime;

		if (GetTile(vNewPlayerPos) == TileType::Coin)
		{
			SetTile(vNewPlayerPos, TileType::Empty);
			nScore++;
		}

		if (GetTile(vNewPlayerPos + vf2d(0.0f, 1.0f)) == TileType::Coin)
		{
			SetTile(vNewPlayerPos + vf2d(0.0f, 1.0f), TileType::Empty);
			nScore++;
		}

		if (GetTile(vNewPlayerPos + vf2d(1.0f, 0.0f)) == TileType::Coin)
		{
			SetTile(vNewPlayerPos + vf2d(1.0f, 0.0f), TileType::Empty);
			nScore++;
		}

		if (GetTile(vNewPlayerPos + vf2d(1.0f, 1.0f)) == TileType::Coin)
		{
			SetTile(vNewPlayerPos + vf2d(1.0f, 1.0f), TileType::Empty);
			nScore++;
		}

		if (vPlayerVel.x <= 0.0f)
		{
			if (GetTile(vi2d(vNewPlayerPos.x + 0.0f, vPlayerPos.y + 0.0f)) != TileType::Empty || GetTile(vi2d(vNewPlayerPos.x + 0.0f, vPlayerPos.y + 0.9f)) != TileType::Empty)
			{
				vNewPlayerPos.x = (float)((int)vNewPlayerPos.x) + 1.0f;
				vPlayerVel.x = 0.0f;
			}
		}
		else
		{
			if (GetTile(vi2d(vNewPlayerPos.x + 1.0f, vPlayerPos.y + 0.0f)) != TileType::Empty || GetTile(vi2d(vNewPlayerPos.x + 1.0f, vPlayerPos.y + 0.9f)) != TileType::Empty)
			{
				vNewPlayerPos.x = (float)((int)vNewPlayerPos.x);
				vPlayerVel.x = 0.0f;
			}
		}

		bPlayerOnGround = false;
		if (vPlayerVel.y <= 0.0f)
		{
			if (GetTile(vNewPlayerPos) != TileType::Empty || GetTile(vNewPlayerPos + vf2d(0.9f, 0.0f)) != TileType::Empty)
			{
				vNewPlayerPos.y = (float)((int)vNewPlayerPos.y) + 1.0f;
				vPlayerVel.y = 0.0f;
			}
		}
		else
		{
			if (GetTile(vNewPlayerPos + vf2d(0.0f, 1.0f)) != TileType::Empty || GetTile(vNewPlayerPos + vf2d(0.9f, 1.0f)) != TileType::Empty)
			{
				vNewPlayerPos.y = (float)((int)vNewPlayerPos.y);
				vPlayerVel.y = 0.0f;
				bPlayerOnGround = true;
			}
		}

		vPlayerPos = vNewPlayerPos;

		DrawSprite(0, 0, sprBackground);

		vi2d vVisibleTiles = ScreenSize() / vTileSize;
		vf2d vOffset = vPlayerPos - (vf2d)vVisibleTiles * 0.5f;

		vOffset = vOffset.max({ 0.0f, 0.0f });
		vOffset = vOffset.min(vLevelSize - vVisibleTiles);

		vf2d vTileOffset = (vOffset - (vf2d)(vi2d)vOffset) * (vf2d)vTileSize;

		vi2d vTile;
		for (vTile.y = -1; vTile.y < vVisibleTiles.y + 1; vTile.y++)
		{
			for (vTile.x = -1; vTile.x < vVisibleTiles.x + 1; vTile.x++)
			{
				TileType tile = GetTile(vTile + (vi2d)vOffset);

				vi2d p = vf2d(vTile * vTileSize) - vTileOffset;

				if (tile != TileType::Empty)
					DrawPartialSprite(p, mapSpriteFileOffsets[tile], vTileSize, sprMario);
			}
		}

		DrawPartialSprite((vPlayerPos - vOffset) * (vf2d)vTileSize, { vTileSize.x * nGraphicID, 0 }, vTileSize, sprMario);

		DrawString(2, 2, "Score: " + to_string(nScore));

		return true;
	}
};

int main()
{
	Mario demo;
	if (demo.Construct(256, 192, 4, 4))
		demo.Run();
	return 0;
}
