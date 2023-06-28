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
	}

	~Mario()
	{
		lua_close(script);

		delete mario;
		delete background;
	}

private:
	lua_State* script;

	vi2d levelSize = { 16, 8 };
	vi2d tileSize = { 32, 32 };

	enum class TileType
	{
		Empty,
		Coin,
		Grass,
		Dirt
	};

	static constexpr size_t TILES_SIZE = 4;

	vector<TileType> level;

	unordered_map<TileType, vi2d> spriteFileOffsets;

	vf2d playerPos = { 1.0f, 1.0f };
	vf2d playerVel = { 0.0f, 0.0f };

	bool isPlayerOnGround = false;

	def::Sprite* mario = nullptr;
	def::Sprite* background = nullptr;

	int graphicID = 0;
	int score = 0;

protected:
	void CreateLevel(const vi2d& size)
	{
		levelSize = size;
		vecLevel.clear();
		vecLevel.resize(levelSize.x * levelSize.y);
	}

	static int wrap_CreateLevel(lua_State* L)
	{
		if (lua_gettop(L) != 3) return -1;

		Mario* obj = static_cast<Mario*>(lua_touserdata(L, 1));
		obj->CreateLevel(vi2d(lua_tointeger(L, 2), lua_tointeger(L, 3)));

		return 0;
	}

	void SetTile(const vi2d& p, TileType tile)
	{
		if (p >= vi2d(0, 0) && p < levelSize)
			vecLevel[p.y * levelSize.x + p.x] = tile;
	}

	static int wrap_SetTile(lua_State* L)
	{
		if (lua_gettop(L) != 4) return -1;

		Mario* obj = static_cast<Mario*>(lua_touserdata(L, 1));
		obj->SetTile(vi2d(lua_tointeger(L, 2), lua_tointeger(L, 3)), TileType(lua_tointeger(L, 4)));

		return 0;
	}

	TileType GetTile(const vi2d& p)
	{
		if (p >= vi2d(0, 0) && p < levelSize)
			return level[p.y * levelSize.x + p.x];

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

						spriteFileOffsets[(TileType)tile] = filePos;
					}
				}
			}
		}

		lua_getglobal(script, "SPRITES_FILE");
		mario = new def::Sprite(lua_tostring(script, -1));

		lua_getglobal(script, "BACKGROUND_FILE");
		background = new def::Sprite(lua_tostring(script, -1));

		lua_getglobal(script, "TILE_SIZE");
		if (lua_istable(script, -1))
		{
			lua_getfield(script, -1, "x");
			tileSize.x = lua_tonumber(script, -1);

			lua_pop(script, 1);

			lua_getfield(script, -1, "y");
			tileSize.y = lua_tonumber(script, -1);

			lua_pop(script, 1);
		}

		return true;
	}

	virtual bool OnUserUpdate(float deltaTime) override
	{
		if (GetWindowState() & def::WS_FOCUSED)
		{
			if (GetKey(def::Key::UP).held)
			{
				playerVel.y = -6.0f;
				graphicID = (playerVel.x >= 0.0f) ? 2 : 3;
			}

			if (GetKey(def::Key::DOWN).held)
				playerVel.y = 6.0f;

			if (GetKey(def::Key::LEFT).held)
			{
				playerVel.x += (isPlayerOnGround ? -25.0f : -15.0f) * deltaTime;
				if (isPlayerOnGround) graphicID = 1;
			}

			if (GetKey(def::Key::RIGHT).held)
			{
				playerVel.x += (isPlayerOnGround ? 25.0f : 15.0f) * deltaTime;
				if (isPlayerOnGround) graphicID = 0;
			}

			if (GetKey(def::Key::SPACE).pressed)
			{
				if (playerVel.y == 0.0f)
					playerVel.y = -12.0f;

				graphicID = (playerVel.x >= 0.0f) ? 2 : 3;
			}
		}

		playerVel.y += 20.0f * deltaTime;

		if (isPlayerOnGround)
		{
			playerVel.x += -3.0f * playerVel.x * deltaTime;
			if (fabs(playerVel.x) < 0.01f) playerVel.x = 0.0f;
		}

		if (playerVel.x > 10.0f) playerVel.x = 10.0f;
		if (playerVel.x < -10.0f) playerVel.x = -10.0f;
		if (playerVel.y > 100.0f) playerVel.y = 100.0f;
		if (playerVel.y < -100.0f) playerVel.y = -100.0f;

		vf2d newPlayerPos = playerPos + playerVel * deltaTime;

		if (GetTile(newPlayerPos) == TileType::Coin)
		{
			SetTile(newPlayerPos, TileType::Empty);
			score++;
		}

		if (GetTile(newPlayerPos + vf2d(0.0f, 1.0f)) == TileType::Coin)
		{
			SetTile(newPlayerPos + vf2d(0.0f, 1.0f), TileType::Empty);
			score++;
		}

		if (GetTile(newPlayerPos + vf2d(1.0f, 0.0f)) == TileType::Coin)
		{
			SetTile(newPlayerPos + vf2d(1.0f, 0.0f), TileType::Empty);
			score++;
		}

		if (GetTile(newPlayerPos + vf2d(1.0f, 1.0f)) == TileType::Coin)
		{
			SetTile(newPlayerPos + vf2d(1.0f, 1.0f), TileType::Empty);
			score++;
		}

		if (playerVel.x <= 0.0f)
		{
			if (GetTile(vi2d(newPlayerPos.x + 0.0f, playerPos.y + 0.0f)) != TileType::Empty || GetTile(vi2d(newPlayerPos.x + 0.0f, playerPos.y + 0.9f)) != TileType::Empty)
			{
				newPlayerPos.x = (float)((int)newPlayerPos.x) + 1.0f;
				playerVel.x = 0.0f;
			}
		}
		else
		{
			if (GetTile(vi2d(newPlayerPos.x + 1.0f, playerPos.y + 0.0f)) != TileType::Empty || GetTile(vi2d(newPlayerPos.x + 1.0f, playerPos.y + 0.9f)) != TileType::Empty)
			{
				newPlayerPos.x = (float)((int)newPlayerPos.x);
				playerVel.x = 0.0f;
			}
		}

		isPlayerOnGround = false;
		if (playerVel.y <= 0.0f)
		{
			if (GetTile(newPlayerPos) != TileType::Empty || GetTile(newPlayerPos + vf2d(0.9f, 0.0f)) != TileType::Empty)
			{
				newPlayerPos.y = (float)((int)newPlayerPos.y) + 1.0f;
				playerVel.y = 0.0f;
			}
		}
		else
		{
			if (GetTile(newPlayerPos + vf2d(0.0f, 1.0f)) != TileType::Empty || GetTile(newPlayerPos + vf2d(0.9f, 1.0f)) != TileType::Empty)
			{
				newPlayerPos.y = (float)((int)newPlayerPos.y);
				playerVel.y = 0.0f;
				isPlayerOnGround = true;
			}
		}

		playerPos = newPlayerPos;

		DrawSprite(0, 0, background);

		vi2d visibleTiles = ScreenSize() / tileSize;
		vf2d offset = playerPos - (vf2d)visibleTiles * 0.5f;

		offset = offset.max({ 0.0f, 0.0f });
		offset = offset.min(levelSize - visibleTiles);

		vf2d tileOffset = (offset - (vf2d)(vi2d)offset) * (vf2d)tileSize;

		vi2d tile;
		for (tile.y = -1; tile.y < visibleTiles.y + 1; tile.y++)
		{
			for (tile.x = -1; tile.x < visibleTiles.x + 1; tile.x++)
			{
				TileType tile = GetTile(tile + (vi2d)offset);

				vi2d p = vf2d(tile * tileSize) - tileOffset;

				if (tile != TileType::Empty)
					DrawPartialSprite(p, mapSpriteFileOffsets[tile], tileSize, sprMario);
			}
		}

		DrawPartialSprite((playerPos - offset) * (vf2d)tileSize, { tileSize.x * graphicID, 0 }, tileSize, mario);

		DrawString(2, 2, "Score: " + to_string(score));

		return true;
	}
};

int main()
{
	Mario demo;
	demo.Construct(256, 192, 4, 4);
	demo.Run();
	return 0;
}
