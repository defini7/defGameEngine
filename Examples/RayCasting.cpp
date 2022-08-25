#include "defGameEngine.h"

class RayCasting : public def::GameEngine
{
public:
	RayCasting()
	{
		SetTitle("Ray Casting");
	}

private:
	float fPlayerX = 2.0f;
	float fPlayerY = 2.0f;

	float fDirX = -1.0f;
	float fDirY = 0.0f;

	float fPlaneX = 0.0f;
	float fPlaneY = 0.66f;

	const int nMapWidth = 16;
	const int nMapHeight = 16;

	std::string map;

protected:
	bool OnUserCreate() override
	{
		map += "################";
		map += "#..............#";
		map += "#...#..........#";
		map += "#...#..........#";
		map += "#...##.........#";
		map += "#....#.........#";
		map += "#..............#";
		map += "#..##..........#";
		map += "#.......########";
		map += "#..............#";
		map += "#..........#...#";
		map += "#........###...#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "################";

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		Clear(def::BLACK);

		for (int x = 0; x < GetScreenWidth(); x++)
		{
			float fCameraX = 2.0f * x / (float)GetScreenWidth() - 1.0f;
			
			float fRayDirX = fDirX + fPlaneX * fCameraX;
			float fRayDirY = fDirY + fPlaneY * fCameraX;

			float fDistanceX = (fRayDirX == 0) ? 1.0f : abs(1 / fRayDirX);
			float fDistanceY = (fRayDirY == 0) ? 1.0f : abs(1 / fRayDirY);

			int nStepX = 0;
			int nStepY = 0;

			bool bHitWall = false;
			int nSide = 0;

			float fFromCurrentDistX;
			float fFromCurrentDistY;
			
			int nMapX = (int)fPlayerX;
			int nMapY = (int)fPlayerY;

			int nPerpWallDistance;

			if (fRayDirX < 0)
			{
				nStepX = -1;
				fFromCurrentDistX = (fPlayerX - (float)nMapX) * fDistanceX;
			}
			else
			{
				nStepX = 1;
				fFromCurrentDistX = ((float)nMapX + 1.0f - fPlayerX) * fDistanceX;
			}

			if (fRayDirY < 0)
			{
				nStepY = -1;
				fFromCurrentDistY = (fPlayerY - (float)nMapY) * fDistanceY;
			}
			else
			{
				nStepY = 1;
				fFromCurrentDistY = ((float)nMapY + 1.0f - fPlayerY) * fDistanceY;
			}

			while (!bHitWall)
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

				if (map[nMapY * nMapWidth + nMapX] == '#')
					bHitWall = true;
			}

			if (nSide == 0)
				nPerpWallDistance = (fFromCurrentDistX - fDistanceX);
			else
				nPerpWallDistance = (fFromCurrentDistY - fDistanceY);
			
			int nLineHeight = 0;

			if (nPerpWallDistance != 0)
				nLineHeight = GetScreenHeight() / nPerpWallDistance;

			int nDrawStart = -nLineHeight / 2 + GetScreenHeight() / 2;

			if (nDrawStart < 0)
				nDrawStart = 0;

			int nDrawEnd = nLineHeight / 2 + GetScreenHeight() / 2;
			
			if (nDrawEnd >= GetScreenHeight())
				nDrawEnd = GetScreenHeight() - 1;

			def::Pixel nPixel;

			switch (map[nMapY * nMapWidth + nMapX])
			{
				case '#': nPixel = def::WHITE; break;
				default: nPixel = def::BLACK; break;
			}

			if (nSide == 1)
			{
				nPixel.r /= 2;
				nPixel.g /= 2;
				nPixel.b /= 2;
				nPixel.a /= 2;
			}

			DrawLine(x, nDrawStart, x, nDrawEnd, nPixel);
		}

		if (GetKey(def::Key::UP).bHeld)
		{
			if (map[int(fPlayerY) * nMapWidth + int(fPlayerX + fDirX * fDeltaTime)] == '.') fPlayerX += fDirX * fDeltaTime;
			if (map[int(fPlayerY + fDirY * fDeltaTime) * nMapWidth + int(fPlayerX)] == '.') fPlayerY += fDirY * fDeltaTime;
		}

		if (GetKey(def::Key::DOWN).bHeld)
		{
			if (map[int(fPlayerY) * nMapWidth + int(fPlayerX - fDirX * fDeltaTime)] == '.') fPlayerX -= fDirX * fDeltaTime;
			if (map[int(fPlayerY - fDirY * fDeltaTime) * nMapWidth + int(fPlayerX)] == '.') fPlayerY -= fDirY * fDeltaTime;
		}

		if (GetKey(def::Key::RIGHT).bHeld)
		{
			float fOldDirX = fDirX;
			float fOldPlaneX = fPlaneX;

			fDirX = fDirX * cosf(-fDeltaTime) - fDirY * sinf(-fDeltaTime);
			fDirY = fOldDirX * sinf(-fDeltaTime) + fDirY * cosf(-fDeltaTime);

			fPlaneX = fPlaneX * cos(-fDeltaTime) - fPlaneY * sin(-fDeltaTime);
			fPlaneY = fOldPlaneX * sin(-fDeltaTime) + fPlaneY * cos(-fDeltaTime);
		}

		if (GetKey(def::Key::LEFT).bHeld)
		{
			float fOldDirX = fDirX;
			float fOldPlaneX = fPlaneX;

			fDirX = fDirX * cosf(fDeltaTime) - fDirY * sinf(fDeltaTime);
			fDirY = fOldDirX * sinf(fDeltaTime) + fDirY * cosf(fDeltaTime);
			
			fPlaneX = fPlaneX * cosf(fDeltaTime) - fPlaneY * sinf(fDeltaTime);
			fPlaneY = fOldPlaneX * sinf(fDeltaTime) + fPlaneY * cosf(fDeltaTime);
		}
		
		return true;
	}

};

int main()
{
	RayCasting demo;
	def::rcode err = demo.Construct(1280, 960);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
