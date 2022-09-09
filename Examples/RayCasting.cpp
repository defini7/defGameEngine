#include "defGameEngine.h"

#define DGE_ANIMATED
#include "DGE_Animated.h"

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
		sMap += "#################.........######";
		sMap += "#..............................#";
		sMap += "#...#...............#..........#";
		sMap += "#...#...............#..........#";
		sMap += "#...##..............##.........#";
		sMap += "#....#...............#.........#";
		sMap += "#..............................#";
		sMap += "#..##..............##..........#";
		sMap += "........#######.........########";
		sMap += "...............................#";
		sMap += "...........#...............#...#";
		sMap += ".........###.............###...#";
		sMap += "...............................#";
		sMap += "...............................#";
		sMap += "...............................#";
		sMap += "#...........#####..............#";
		sMap += "#..............................#";
		sMap += "#..............................#";
		sMap += "#...#...............#..........#";
		sMap += "#...#...............#..........#";
		sMap += "#...##..............##.........#";
		sMap += "#....#...............#.........#";
		sMap += "#..............................#";
		sMap += "#..##..............##..........#";
		sMap += "#.......#######.........########";
		sMap += "#...............................";
		sMap += "#..........#...............#....";
		sMap += "#........###.............###....";
		sMap += "#...............................";
		sMap += "#...............................";
		sMap += "#...............................";
		sMap += "######..................########";

		sprWall = new def::Sprite("wall.png");

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

			float fDistanceX = sqrtf(1.0f + (fRayDirY / fRayDirX) * (fRayDirY / fRayDirX));
			float fDistanceY = sqrtf(1.0f + (fRayDirX / fRayDirY) * (fRayDirX / fRayDirY));

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

				if (nMapY < 0 || nMapY >= nMapWidth)
					bNoWall = true;

				if (!bNoWall && sMap[nMapY * nMapWidth + nMapX] == '#')
					bHitWall = true;
			}

			if (nSide == 0)
				fPerpWallDistance = fFromCurrentDistX - fDistanceX;
			else
				fPerpWallDistance = fFromCurrentDistY - fDistanceY;
			
			int nLineHeight = int((float)h / fPerpWallDistance);

			int nDrawStart = -nLineHeight / 2 + h / 2;
			int nDrawEnd = nLineHeight / 2 + h / 2;

			if (nDrawStart < 0)
				nDrawStart = 0;

			if (nDrawEnd >= h)
				nDrawEnd = h - 1;

			int nCeiling = float(h / 2) - h / (fPerpWallDistance);
			int nFloor = h - nCeiling;

			float fBlockMidX = fFromCurrentDistX + 0.5f;
			float fBlockMidY = fFromCurrentDistY + 0.5f;

			float fTestPointX = fPlayerX + fRayDirX * fDistanceX;
			float fTestPointY = fPlayerY + fRayDirY * fDistanceY;

			float fTestAngle = atan2f(fTestPointY - fBlockMidY, fTestPointX - fBlockMidX);

			float fSampleX = 0.0f, fSampleY = 0.0f;

			if (fPerpWallDistance < fDepth)
			{
				if (fTestAngle >= (float)M_PI * 0.25f && fTestAngle < (float)M_PI * 0.25f)
					fSampleX = fTestPointY - fFromCurrentDistY;
			}

			for (int y = 0; y < h; y++)
			{
				if (y <= nCeiling) // sky
					Draw(x, y, def::BLACK);
				else if (y > nCeiling && y < nFloor && !bNoWall) // wall
				{
					//if (nPerpWallDistance <= nDepth / 4)			pWallPixel = def::WHITE;	// close
					//else if (nPerpWallDistance < nDepth / 3)		pWallPixel = def::GREY;
					//else if (nPerpWallDistance < nDepth / 2)		pWallPixel = def::DARK_GREY;
					//else if (nPerpWallDistance < nDepth)			pWallPixel = def::DARK_GREY;
					//else											pWallPixel = def::BLACK;    // far

					if (fPerpWallDistance < fDepth)
					{
						fSampleY = ((float)y - (float)nCeiling) / ((float)nFloor - (float)nCeiling);
						Draw(x, y, sprWall->Sample(fSampleX, fSampleY));
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
				switch (sMap[y * nMapWidth + x])
				{
				case '#': FillRectangle(x * nCellSize, y * nCellSize, nCellSize, nCellSize, def::WHITE); break;
				case '.': FillRectangle(x * nCellSize, y * nCellSize, nCellSize, nCellSize, def::GREY); break;
				}
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
