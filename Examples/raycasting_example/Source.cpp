#include "defGameEngine.h"

#define DGE_ANIMATED
#include "DGE_Animated.h"

#define DGE_RAYCASTING
#include "DGE_RayCasting.h"

#include <algorithm>
#include <cmath>

class RayCasting : public def::GameEngine, def::DGE_RayCasting
{
public:
	RayCasting()
	{
		SetTitle("Ray Casting");
	}

private:
	def::Sprite* sprTiles = nullptr;

	int nTexWidth = 64;
	int nTexHeight = 64;

	def::Sprite* sprGun = nullptr;
	def::Texture* gfxGun = nullptr;

	def::Animated* anim = nullptr; // for animations

	enum TEXTURES
	{
		BLUESTONE,
		COLORSTONE,
		EAGLE,
		GREYSTONE,
		MOSSY,
		PILLAR,
		PURPLESTONE,
		REDBRICK,
		WOOD,
		BARREL,
		BULLET
	};

	uint32_t nGunId;

	bool bShooting = false;

	def::vi2d vFilePos = def::vi2d(0, 0);
	def::vi2d vFileSize = def::vi2d(0, 0);

	int nCrosshairWidth = 10;
	int nCrosshairHeight = 10;

	int nMapCellSize = 3;

	def::Pixel pCrosshairColor = def::WHITE;

	TEXTURES nFloorId = GREYSTONE;
	TEXTURES nCeilingId = WOOD;

	std::string sMap;

	int nMapWidth = 32;
	int nMapHeight = 32;

protected:
	def::Pixel ApplyWorldPixel(def::Side side, int tex_x, int tex_y, int map_x, int map_y, bool is_wall) override
	{
		switch (side)
		{
		case def::Side::CEILING: return sprTiles->GetPixel(nCeilingId * nTexWidth + tex_x, tex_y);
		case def::Side::WALL:	 return is_wall ? sprTiles->GetPixel(((int)sMap[map_y * nMapWidth + map_x] - 48) * nTexWidth + tex_x, tex_y) : def::BLACK;
		case def::Side::FLOOR:	 return sprTiles->GetPixel(nFloorId * nTexWidth + tex_x, tex_y);
		}
	}

	def::Pixel ApplyObjectPixel(uint32_t id, int x, int y) override
	{
		return sprTiles->GetPixel(id * nTexWidth + x, y);
	}

	void DrawPixel(int x, int y, def::Pixel p) override
	{
		Draw(x, y, p);
	}

	bool GetKeyState(unsigned int kc, def::State s) override
	{
		switch (s)
		{
		case def::State::HELD:		return GetKey(kc).bHeld;
		case def::State::PRESSED:	return GetKey(kc).bPressed;
		case def::State::RELEASED:	return GetKey(kc).bReleased;
		}
	}

	bool OnUserCreate() override
	{
		Setup(GetScreenWidth(), GetScreenHeight());

		SetKeys(def::Key::W, def::Key::S, def::Key::A, def::Key::D);

		sMap += "77777777777777777.........777777";
		sMap += "7..............................7";
		sMap += "7...8...............8..........7";
		sMap += "7...8...............8..........7";
		sMap += "7...88..............88.........7";
		sMap += "7....8...............8.........7";
		sMap += "7..............................7";
		sMap += "7..88..............88..........7";
		sMap += "7.......8888888.........88888887";
		sMap += "7..............................7";
		sMap += "...........8...............8...7";
		sMap += ".........888.............888...7";
		sMap += "...............................2";
		sMap += "...............................7";
		sMap += "...............................7";
		sMap += "............88888..............7";
		sMap += "...............................7";
		sMap += "7..............................7";
		sMap += "7...6...............6..........7";
		sMap += "7...6...............6..........7";
		sMap += "7...66..............66.........7";
		sMap += "7....6...............6.........7";
		sMap += "7..............................7";
		sMap += "7..66..............66..........7";
		sMap += "7.......6666666.........66666667";
		sMap += "7...............................";
		sMap += "7..........6...............6....";
		sMap += "7........666.............666....";
		sMap += "7...............................";
		sMap += "7...............................";
		sMap += "7...............................";
		sMap += "777777..................77777777";

		SetMap(sMap, nMapWidth, nMapHeight);

		SetTexWidth(nTexWidth);
		SetTexHeight(nTexHeight);

		sprTiles = new def::Sprite("gfx/tileset.png");

		sprGun = new def::Sprite("gfx/gun.png");
		gfxGun = new def::Texture(sprGun);

		anim = new def::Animated();

		nGunId = anim->AddAnimation(def::vi2d(0, 0), def::vi2d(128, 128), 6);

		AddObject({ 8.5f, 8.5f, 0.0f, 0.0f, 0.0f, BARREL });
		AddObject({ 7.5f, 7.5f, 0.0f, 0.0f, 0.0f, BARREL });
		AddObject({ 10.0f, 3.0f, 0.0f, 0.0f, 0.0f, BARREL });

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		Clear(def::BLACK);

		SetDeltaTime(fDeltaTime);
		DoRayCasting();

		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				if (sMap[y * nMapWidth + x] == '.')
					Draw(x, y, def::GREY);
				else
					Draw(x, y, def::WHITE);
			}

		Draw((int)GetPlayerX(), (int)GetPlayerY(), def::YELLOW);

		if (bShooting)
		{
			bShooting = !anim->Animate(nGunId, vFilePos, vFileSize, 10.0f * fDeltaTime);

			if ((vFilePos.x == 0 && vFilePos.y) || (vFilePos.x + vFileSize.x > sprGun->GetWidth()) || (vFilePos.y + vFileSize.y > sprGun->GetHeight()))
				vFilePos = def::vi2d(0, 0);

			if (vFileSize.x == 0 && vFileSize.y == 0)
				vFileSize = def::vi2d(128, 128);

			DrawPartialTexture(GetScreenSize<int>() - 128, vFilePos, vFileSize, gfxGun);
		}
		else
			DrawPartialTexture(GetScreenSize<int>() - 128, def::vi2d(0, 0), def::vi2d(128, 128), gfxGun);

		// Draw crosshair

		DrawLine(
			GetScreenWidth() / 2 - nCrosshairWidth / 2, GetScreenHeight() / 2,
			GetScreenWidth() / 2 + nCrosshairWidth / 2, GetScreenHeight() / 2,
			pCrosshairColor
		);

		DrawLine(
			GetScreenWidth() / 2, GetScreenHeight() / 2 - nCrosshairHeight / 2,
			GetScreenWidth() / 2, GetScreenHeight() / 2 + nCrosshairHeight / 2,
			pCrosshairColor
		);

		if (GetMouse(0).bPressed)
		{
			bShooting = true;

			def::sObject o;

			o.x = GetPlayerX();
			o.y = GetPlayerY();

			o.speed = 5.0f;

			o.vx = GetDirX();
			o.vy = GetDirY();

			o.id = BULLET;

			AddObject(o);
		}

		ManipulateObjects([&](std::pair<def::sObject*, def::sObject*> o) {
			if ((int)round(o.first->x) == (int)round(o.second->x) && (int)round(o.first->y) == (int)round(o.second->y)) // check for collision
			{
				if (o.first->id == BULLET || o.second->id == BULLET)
				{
					o.first->bRemove = true;
					o.second->bRemove = true;
				}
			}

			return o;
		});

		return true;
	}

};

int main()
{
	RayCasting demo;

	if (demo.Construct(256, 240, 4, 4, false, true).ok)
		demo.Run();

	return 0;
}
