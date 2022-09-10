#include "defGameEngine.h"

#define DGE_ANIMATED
#include "DGE_Animated.h"

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
	BARREL
};

class RayCasting : public def::GameEngine
{
public:
	RayCasting()
	{
		SetTitle("Ray Casting");
	}

private:
	float fPlayerX = 5.0f;
	float fPlayerY = 5.0f;

	float fMoveSpeed = 5.0f;
	float fRotSpeed = 3.0f;

	float fDirX = -1.0f;
	float fDirY = 0.0f;

	float fPlaneX = 0.0f;
	float fPlaneY = 0.66f; // 66 degrees

	const int nMapWidth = 32;
	const int nMapHeight = 32;

	float fDepth = 16.0f;

	std::string sMap;

	def::Sprite* sprWall = nullptr;

	const int nTexWidth = 64;
	const int nTexHeight = 64;

	def::Sprite* sprGun = nullptr;
	def::GFX* gfxGun = nullptr;

	def::Animated* anim = nullptr; // for animations

	uint32_t id_gun;

	bool bShooting = false;

	def::vi2d vFilePos = def::vi2d(0, 0);
	def::vi2d vFileSize = def::vi2d(0, 0);

protected:
	bool OnUserCreate() override
	{
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

		sprWall = new def::Sprite("gfx/tileset.png");

		sprGun = new def::Sprite("gun.png");
		gfxGun = new def::GFX(sprGun, GetRenderer());

		anim = new def::Animated(gfxGun);

		id_gun = anim->AddAnimation(def::vi2d(0, 0).ref(), def::vi2d(128, 128).ref(), 6);

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		Clear(def::BLACK);

		int h = GetScreenHeight();

		for (int x = 0; x < GetScreenWidth(); x++)
		{
			float fCameraX = 2.0f * x / (float)GetScreenWidth() - 1.0f;

			float fRayDirX = fDirX + fPlaneX * fCameraX;
			float fRayDirY = fDirY + fPlaneY * fCameraX;

			/*float fDistanceX = sqrtf(1.0f + (fRayDirY / fRayDirX) * (fRayDirY / fRayDirX));
			float fDistanceY = sqrtf(1.0f + (fRayDirX / fRayDirY) * (fRayDirX / fRayDirY));*/

			float fDistanceX = fabs(1.0f / fRayDirX);
			float fDistanceY = fabs(1.0f / fRayDirY);

			int nStepX = 0;
			int nStepY = 0;

			bool bHitWall = false;
			bool bNoWall = false;
			int nSide = 0;

			float fFromCurrentDistX;
			float fFromCurrentDistY;

			int nMapX = (int)fPlayerX;
			int nMapY = (int)fPlayerY;

			float fPerpWallDistance;

			if (fRayDirX < 0.0f)
			{
				nStepX = -1;
				fFromCurrentDistX = (fPlayerX - (float)nMapX) * fDistanceX;
			}
			else
			{
				nStepX = 1;
				fFromCurrentDistX = ((float)nMapX + 1.0f - fPlayerX) * fDistanceX;
			}

			if (fRayDirY < 0.0f)
			{
				nStepY = -1;
				fFromCurrentDistY = (fPlayerY - (float)nMapY) * fDistanceY;
			}
			else
			{
				nStepY = 1;
				fFromCurrentDistY = ((float)nMapY + 1.0f - fPlayerY) * fDistanceY;
			}

			int nTexId = -1;

			while (!bHitWall && !bNoWall)
			{
				if (fFromCurrentDistX < fFromCurrentDistY)
				{
					fFromCurrentDistX += fDistanceX;
					nMapX += nStepX;
					nSide = 0;
				}
				else
				{
					fFromCurrentDistY += fDistanceY;
					nMapY += nStepY;
					nSide = 1;
				}

				if (nMapY < 0 || nMapY >= nMapHeight || nMapX < 0 || nMapX >= nMapWidth)
					bNoWall = true;

				if (!bNoWall && std::isdigit(sMap[nMapY * nMapWidth + nMapX]))
				{
					nTexId = (int)sMap[nMapY * nMapWidth + nMapX] - 48;
					bHitWall = true;
				}
			}

			if (nSide == 0)
				fPerpWallDistance = fFromCurrentDistX - fDistanceX;
			else
				fPerpWallDistance = fFromCurrentDistY - fDistanceY;
			
			int nLineHeight = int((float)h / fPerpWallDistance) * 2; // change '2' to something and see what happens

			int nCeiling = float(h / 2) - h / (fPerpWallDistance);
			int nFloor = h - nCeiling;

			float fTestPoint;

			if (nSide == 0)
				fTestPoint = fPlayerY + fRayDirY * fPerpWallDistance;
			else
				fTestPoint = fPlayerX + fRayDirX * fPerpWallDistance;

			fTestPoint -= floorf(fTestPoint);

			int nTexX = int(fTestPoint * 64.0f);

			if ((nSide == 0 && fRayDirX > 0.0f) || (nSide == 1 && fRayDirY < 0.0f))
				nTexX = nTexWidth - nTexX - 1;

			float fTexStep = (float)nTexHeight / (float)nLineHeight;
			float fTexPos = float(nFloor - h / 2 + nLineHeight / 2) * fTexStep;

			for (int y = 0; y < h; y++)
			{
				if (y <= nCeiling) // sky
					Draw(x, y, def::BLACK);
				else if (y > nCeiling && y <= nFloor && !bNoWall) // wall
				{
					if (fPerpWallDistance < fDepth && nTexId != -1)
					{
						int nTexY = (int)fTexPos & (nTexHeight - 1);
						fTexPos += fTexStep;

						def::Pixel p = sprWall->GetPixel(nTexId * nTexWidth + nTexX, nTexY);

						if (nTexId == 3)
							std::cout << nTexId << '\n';

						if (nSide == 1)
						{
							p.r = (p.r >> 1) & 8355711;
							p.g = (p.g >> 1) & 8355711;
							p.b = (p.b >> 1) & 8355711;
						}

						Draw(x, y, p);
					}
				}
				else
				{
					def::Pixel pFloorPixel;

					float b = 1.0f - ((y - float(h / 2)) / (float(h / 2)));
					if (b < 0.25f)		pFloorPixel = def::CYAN;
					else if (b < 0.5f)	pFloorPixel = def::DARK_CYAN;
					else if (b < 0.75f)	pFloorPixel = def::BLUE;
					else if (b < 0.9f)	pFloorPixel = def::DARK_BLUE;
					else				pFloorPixel = def::BLACK;

					Draw(x, y, pFloorPixel);
				}
			}
		}

		int nCellSize = 3;

		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				if (sMap[y * nMapWidth + x] == '.')
					FillRectangle(x * nCellSize, y * nCellSize, nCellSize, nCellSize, def::GREY);
				else
					FillRectangle(x* nCellSize, y* nCellSize, nCellSize, nCellSize, def::WHITE);
			}

		FillRectangle((int)fPlayerX * nCellSize, (int)fPlayerY * nCellSize, nCellSize, nCellSize, def::YELLOW);

		auto draw_default_gun = [&]()
		{
			DrawPartialGFX(GetScreenWidth() - 256, GetScreenHeight() - 256, 0, 0, 128, 128, gfxGun, 0.0f, 2.0f);
		};

		if (bShooting)
		{
			bShooting = !anim->Animate(id_gun, vFilePos, vFileSize, 10.0f * fDeltaTime);
			
			if (vFileSize == 0)
				draw_default_gun();
			else
				DrawPartialGFX(GetScreenWidth() - 256, GetScreenHeight() - 256, vFilePos.x, vFilePos.y, vFileSize.x, vFileSize.y, gfxGun, 0.0f, 2.0f);
		}
		else
			draw_default_gun();

		if (GetKey(def::Key::W).bHeld)
		{
			float fNewX = fPlayerX + fDirX * fMoveSpeed * fDeltaTime;
			float fNewY = fPlayerY + fDirY * fMoveSpeed * fDeltaTime;

			if ((int)fNewX < nMapWidth && (int)fNewX >= 0 && (int)fNewY < nMapHeight && (int)fNewY >= 0)
			{
				if (sMap[(int)fNewY * nMapWidth + (int)fNewX] == '.') fPlayerX = fNewX;
				if (sMap[(int)fNewY * nMapWidth + (int)fNewX] == '.') fPlayerY = fNewY;
			}
		}

		if (GetKey(def::Key::S).bHeld)
		{
			float fNewX = fPlayerX - fDirX * fMoveSpeed * fDeltaTime;
			float fNewY = fPlayerY - fDirY * fMoveSpeed * fDeltaTime;

			if ((int)fNewX < nMapWidth && (int)fNewX >= 0 && (int)fNewY < nMapHeight && (int)fNewY >= 0)
			{
				if (sMap[(int)fNewY * nMapWidth + (int)fNewX] == '.') fPlayerX = fNewX;
				if (sMap[(int)fNewY * nMapWidth + (int)fNewX] == '.') fPlayerY = fNewY;
			}
		}

		if (GetKey(def::Key::A).bHeld)
		{
			float fOldDirX = fDirX;
			float fOldPlaneX = fPlaneX;

			fDirX = fDirX * cosf(fRotSpeed * fDeltaTime) - fDirY * sinf(fRotSpeed * fDeltaTime);
			fDirY = fOldDirX * sinf(fRotSpeed * fDeltaTime) + fDirY * cosf(fRotSpeed * fDeltaTime);

			fPlaneX = fPlaneX * cosf(fRotSpeed * fDeltaTime) - fPlaneY * sinf(fRotSpeed * fDeltaTime);
			fPlaneY = fOldPlaneX * sinf(fRotSpeed * fDeltaTime) + fPlaneY * cosf(fRotSpeed * fDeltaTime);
		}

		if (GetKey(def::Key::D).bHeld)
		{
			float fOldDirX = fDirX;
			float fOldPlaneX = fPlaneX;

			fDirX = fDirX * cosf(-fRotSpeed * fDeltaTime) - fDirY * sinf(-fRotSpeed * fDeltaTime);
			fDirY = fOldDirX * sinf(-fRotSpeed * fDeltaTime) + fDirY * cosf(-fRotSpeed * fDeltaTime);

			fPlaneX = fPlaneX * cosf(-fRotSpeed * fDeltaTime) - fPlaneY * sinf(-fRotSpeed * fDeltaTime);
			fPlaneY = fOldPlaneX * sinf(-fRotSpeed * fDeltaTime) + fPlaneY * cosf(-fRotSpeed * fDeltaTime);
		}

		if (GetMouse(0).bPressed)
			bShooting = true;

		return true;
	}

};

int main()
{
	RayCasting demo;

	if (demo.Construct(800, 600).ok)
		demo.Run();

	return 0;
}
