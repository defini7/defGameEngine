#include "defGameEngine.h"

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

	float fDirX = -1.0f;
	float fDirY = 0.0f;

	float fPlaneX = 0.0f;
	float fPlaneY = 0.66f;

	const int nMapWidth = 32;
	const int nMapHeight = 32;

	int nDepth = 16;

	std::string map;

protected:
	bool OnUserCreate() override
	{
		map += "#################.........######";
		map += "#..............................#";
		map += "#...#...............#..........#";
		map += "#...#...............#..........#";
		map += "#...##..............##.........#";
		map += "#....#...............#.........#";
		map += "#..............................#";
		map += "#..##..............##..........#";
		map += "........#######.........########";
		map += "...............................#";
		map += "...........#...............#...#";
		map += ".........###.............###...#";
		map += "...............................#";
		map += "...............................#";
		map += "...............................#";
		map += "#...........#####..............#";
		map += "#..............................#";
		map += "#..............................#";
		map += "#...#...............#..........#";
		map += "#...#...............#..........#";
		map += "#...##..............##.........#";
		map += "#....#...............#.........#";
		map += "#..............................#";
		map += "#..##..............##..........#";
		map += "#.......#######.........########";
		map += "#...............................";
		map += "#..........#...............#....";
		map += "#........###.............###....";
		map += "#...............................";
		map += "#...............................";
		map += "#...............................";
		map += "######..................########";

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

			float fDistanceX = (fRayDirX == 0) ? 1e30f : fabs(1.0f / fRayDirX);
			float fDistanceY = (fRayDirY == 0) ? 1e30f : fabs(1.0f / fRayDirY);
			
			int nStepX = 0;
			int nStepY = 0;

			bool bHitWall = false;
			bool bNoWall = false;
			int nSide = 0;

			float fFromCurrentDistX;
			float fFromCurrentDistY;

			int nMapX = (int)fPlayerX;
			int nMapY = (int)fPlayerY;

			int nPerpWallDistance;

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

				if (!bNoWall && map[nMapY * nMapWidth + nMapX] == '#')
					bHitWall = true;
			}

			if (nSide == 0)
				nPerpWallDistance = (fFromCurrentDistX - fDistanceX);
			else
				nPerpWallDistance = (fFromCurrentDistY - fDistanceY);

			int nLineHeight = int((float)h / (float)nPerpWallDistance);

			int nDrawStart = -nLineHeight / 2 + h / 2;

			if (nDrawStart < 0)
				nDrawStart = 0;

			int nDrawEnd = nLineHeight / 2 + h / 2;

			if (nDrawEnd >= h)
				nDrawEnd = h - 1;

			int nCeiling = float(h / 2) - h / ((float)nPerpWallDistance);
			int nFloor = h - nCeiling;

			for (int y = 0; y < h; y++)
			{
				if (y <= nCeiling) // sky
					Draw(x, y, def::BLACK);
				else if (y > nCeiling && y < nFloor && !bNoWall) // wall
				{
					def::Pixel pWallPixel;

					if (nPerpWallDistance <= nDepth / 4)			pWallPixel = def::WHITE;	// close
					else if (nPerpWallDistance < nDepth / 3)		pWallPixel = def::GREY;
					else if (nPerpWallDistance < nDepth / 2)		pWallPixel = def::DARK_GREY;
					else if (nPerpWallDistance < nDepth)			pWallPixel = def::DARK_GREY;
					else											pWallPixel = def::BLACK;    // far

					Draw(x, y, pWallPixel);
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
				switch (map[y * nMapWidth + x])
				{
				case '#': FillRectangle(x * nCellSize, y * nCellSize, nCellSize, nCellSize, def::WHITE); break;
				case '.': FillRectangle(x * nCellSize, y * nCellSize, nCellSize, nCellSize, def::GREY); break;
				}
			}

		FillRectangle((int)fPlayerX * nCellSize, (int)fPlayerY * nCellSize, nCellSize, nCellSize, def::YELLOW);

		float fSpeed = 5.0f * fDeltaTime;
		float fRotSpeed = 3.0f * fDeltaTime;

		if (GetKey(def::Key::UP).bHeld)
		{
			float fNewX = fPlayerX + fDirX * fSpeed;
			float fNewY = fPlayerY + fDirY * fSpeed;

			if ((int)fNewX < nMapWidth && (int)fNewX >= 0 && (int)fNewY < nMapHeight && (int)fNewY >= 0)
			{
				if (map[(int)fNewY * nMapWidth + (int)fNewX] == '.') fPlayerX = fNewX;
				if (map[(int)fNewY * nMapWidth + (int)fNewX] == '.') fPlayerY = fNewY;
			}
		}

		if (GetKey(def::Key::DOWN).bHeld)
		{
			float fNewX = fPlayerX - fDirX * fSpeed;
			float fNewY = fPlayerY - fDirY * fSpeed;

			if ((int)fNewX < nMapWidth && (int)fNewX >= 0 && (int)fNewY < nMapHeight && (int)fNewY >= 0)
			{
				if (map[(int)fNewY * nMapWidth + (int)fNewX] == '.') fPlayerX = fNewX;
				if (map[(int)fNewY * nMapWidth + (int)fNewX] == '.') fPlayerY = fNewY;
			}
		}

		if (GetKey(def::Key::A).bHeld)
		{
			float fOldDirX = fDirX;
			float fOldPlaneX = fPlaneX;

			fDirX = fDirX * cosf(fRotSpeed) - fDirY * sinf(fRotSpeed);
			fDirY = fOldDirX * sinf(fRotSpeed) + fDirY * cosf(fRotSpeed);

			fPlaneX = fPlaneX * cosf(fRotSpeed) - fPlaneY * sinf(fRotSpeed);
			fPlaneY = fOldPlaneX * sinf(fRotSpeed) + fPlaneY * cosf(fRotSpeed);
		}

		if (GetKey(def::Key::D).bHeld)
		{
			float fOldDirX = fDirX;
			float fOldPlaneX = fPlaneX;

			fDirX = fDirX * cosf(-fRotSpeed) - fDirY * sinf(-fRotSpeed);
			fDirY = fOldDirX * sinf(-fRotSpeed) + fDirY * cosf(-fRotSpeed);

			fPlaneX = fPlaneX * cosf(-fRotSpeed) - fPlaneY * sinf(-fRotSpeed);
			fPlaneY = fOldPlaneX * sinf(-fRotSpeed) + fPlaneY * cosf(-fRotSpeed);
		}

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
