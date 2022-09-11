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
	float fPlayerX = 7.0f;
	float fPlayerY = 3.0f;

	float fMoveSpeed = 5.0f;
	float fRotSpeed = 3.0f;

	float fDirX = -1.0f;
	float fDirY = 0.0f;

	float fPlaneX = 0.0f;
	float fPlaneY = 0.66f; // 66 degrees

	int nMapWidth = 32;
	int nMapHeight = 32;

	float fDepth = 16.0f;

	std::string sMap;

	def::Sprite* sprTiles = nullptr;

	int nTexWidth = 64;
	int nTexHeight = 64;

	def::Sprite* sprGun = nullptr;
	def::GFX* gfxGun = nullptr;

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
		BARREL
	};

	uint32_t nGunId;

	bool bShooting = false;

	def::vi2d vFilePos = def::vi2d(0, 0);
	def::vi2d vFileSize = def::vi2d(0, 0);

	int nCrosshairWidth = 10;
	int nCrosshairHeight = 10;

	int nMapCellSize = 3;
	
	def::Pixel pCrosshairColor = def::MAGENTA;

	struct sObject
	{
		float x;
		float y;

		TEXTURES id;
	};

	std::list<sObject> listObjects;

	float* fDepthBuffer = nullptr;

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

		sprTiles = new def::Sprite("gfx/tileset.png");

		sprGun = new def::Sprite("gun.png");
		gfxGun = new def::GFX(sprGun, GetRenderer());

		anim = new def::Animated(gfxGun);

		nGunId = anim->AddAnimation(def::vi2d(0, 0).ref(), def::vi2d(128, 128).ref(), 6);

		listObjects = {
			{ 8.5f, 8.5f, BARREL },
			{ 7.5f, 7.5f, BARREL },
			{ 10.0f, 3.0f, BARREL }
		};

		fDepthBuffer = new float[GetScreenWidth()];

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

			float fDistanceToWall;

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
				{
					fDistanceToWall = fDepth;
					bNoWall = true;
				}

				if (!bNoWall && std::isdigit(sMap[nMapY * nMapWidth + nMapX]))
				{
					nTexId = (int)sMap[nMapY * nMapWidth + nMapX] - 48;
					bHitWall = true;
				}
			}

			if (nSide == 0)
				fDistanceToWall = fFromCurrentDistX - fDistanceX;
			else
				fDistanceToWall = fFromCurrentDistY - fDistanceY;

			int nLineHeight = int((float)GetScreenHeight() / fDistanceToWall) * 2;

			int nCeiling = -nLineHeight / 2 + GetScreenHeight() / 2;
			int nFloor = nLineHeight / 2 + GetScreenHeight() / 2;

			if (nCeiling < 0)
				nCeiling = 0;

			if (nFloor > GetScreenHeight())
				nFloor = GetScreenHeight();

			float fTestPoint;

			if (nSide == 0)
				fTestPoint = fPlayerY + fRayDirY * fDistanceToWall;
			else
				fTestPoint = fPlayerX + fRayDirX * fDistanceToWall;

			fTestPoint -= floorf(fTestPoint);

			int nTexX = int(fTestPoint * (float)nTexWidth);

			if ((nSide == 0 && fRayDirX > 0.0f) || (nSide == 1 && fRayDirY < 0.0f))
				nTexX = nTexWidth - nTexX - 1;

			float fTexStep = (float)nTexHeight / (float)nLineHeight;
			float fTexPos = float(nCeiling - GetScreenHeight() / 2 + nLineHeight / 2) * fTexStep;

			for (int y = 0; y < GetScreenHeight(); y++)
			{
				if (y <= nCeiling) // sky
					Draw(x, y, def::BLACK);
				else if (y > nCeiling && y <= nFloor && !bNoWall) // wall
				{
					if (fDistanceToWall < fDepth && nTexId != -1)
					{
						int nTexY = (int)fTexPos & (nTexHeight - 1);
						fTexPos += fTexStep;

						def::Pixel pWallPixel = sprTiles->GetPixel(nTexId * nTexWidth + nTexX, nTexY);
						
						if (fDistanceToWall > 2.0f)
						{
							pWallPixel.r = uint8_t((float)pWallPixel.r / fDistanceToWall * 2.0f);
							pWallPixel.g = uint8_t((float)pWallPixel.g / fDistanceToWall * 2.0f);
							pWallPixel.b = uint8_t((float)pWallPixel.b / fDistanceToWall * 2.0f);
						}

						Draw(x, y, pWallPixel);
					}
				}
				else
				{
					float b = 1.0f - ((y - float(GetScreenHeight() / 2)) / (float(GetScreenHeight() / 2)));

					if (b < 0.9f) // floor
					{
						def::Pixel pFloorPixel = def::CYAN;

						if (fDistanceToWall > 2.0f)
						{
							pFloorPixel.r = uint8_t((float)pFloorPixel.r / fDistanceToWall * 2.0f);
							pFloorPixel.g = uint8_t((float)pFloorPixel.g / fDistanceToWall * 2.0f);
							pFloorPixel.b = uint8_t((float)pFloorPixel.b / fDistanceToWall * 2.0f);
						}

						Draw(x, y, pFloorPixel);
					}
				}
			}

			fDepthBuffer[x] = fDistanceToWall;
		}

		for (auto& o : listObjects)
		{
			float fObjDirX = o.x - fPlayerX;
			float fObjDirY = o.y - fPlayerY;

			for (int ox = (int)o.x; ox < (int)o.x + nTexWidth; ox++)
			{
				float fCameraX = 2.0f * fDepthBuffer[ox] / (float)GetScreenWidth() - 1.0f;

				float fEyeX = fDirX + fPlaneX * fCameraX;
				float fEyeY = fDirY + fPlaneY * fCameraX;
				
				float fDistanceFromPlayer = sqrtf(fObjDirX * fObjDirX + fObjDirY * fObjDirY);

				float fObjAngle = atan2f(fEyeY, fEyeX) - atan2f(fObjDirY, fObjDirX);

				if (fObjAngle < -3.14159f)
					fObjAngle += 2.0f * 3.14159f;

				if (fObjAngle > 3.14159f)
					fObjAngle -= 2.0f * 3.14159f;

				float fObjCeiling = float(GetScreenHeight() / 2) - (float)GetScreenHeight() / fDistanceFromPlayer;
				float fObjFloor = (float)GetScreenHeight() - fObjCeiling;
				float fObjMiddle = 0.5f * (fObjAngle / (fPlaneY / 2.0f) + 0.5f) * (float)GetScreenWidth();

				if (fabs(fObjAngle) < fPlaneY / 2.0f)
				{
					if (fDistanceFromPlayer >= 0.5f && fDistanceFromPlayer < fDepth)
					{
						for (int oy = (int)o.y; oy < (int)o.y + nTexHeight; oy++)
						{
							int nObjectColumn = int(fObjMiddle + (float)ox - nTexWidth / 2.0f);

							if (nObjectColumn >= 0 && nObjectColumn < GetScreenWidth())
								Draw(nObjectColumn, (int)fObjCeiling + oy, sprTiles->GetPixel(nTexWidth * o.id + ox, oy));
						}
					}
				}
			}		
		}

		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				if (sMap[y * nMapWidth + x] == '.')
					FillRectangle(x * nMapCellSize, y * nMapCellSize, nMapCellSize, nMapCellSize, def::GREY);
				else
					FillRectangle(x* nMapCellSize, y* nMapCellSize, nMapCellSize, nMapCellSize, def::WHITE);
			}

		FillRectangle((int)fPlayerX * nMapCellSize, (int)fPlayerY * nMapCellSize, nMapCellSize, nMapCellSize, def::YELLOW);

		if (bShooting)
		{
			bShooting = !anim->Animate(nGunId, vFilePos, vFileSize, 10.0f * fDeltaTime);
			
			if (vFileSize == 0)
				vFileSize = def::vi2d(128, 128);

			DrawPartialGFX(GetScreenWidth() - 256, GetScreenHeight() - 256, vFilePos.x, vFilePos.y, vFileSize.x, vFileSize.y, gfxGun, 0.0f, 2.0f);
		}
		else
			DrawPartialGFX(GetScreenWidth() - 256, GetScreenHeight() - 256, 0, 0, 128, 128, gfxGun, 0.0f, 2.0f);

		// Draw crosshair
		
		DrawLine(
			GetScreenWidth() / 2 - nCrosshairWidth / 2, GetScreenHeight() / 2,
			GetScreenWidth() / 2 + nCrosshairWidth / 2, GetScreenHeight() / 2
		);

		DrawLine(
			GetScreenWidth() / 2, GetScreenHeight() / 2 - nCrosshairHeight / 2,
			GetScreenWidth() / 2, GetScreenHeight() / 2 + nCrosshairHeight / 2
		);

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
