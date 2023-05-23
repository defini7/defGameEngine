#define DGE_APPLICATION
#include "defGameEngine.h"

#include <algorithm>

struct Object
{
	def::vf2d vPos;
	def::vf2d vVel;

	float fSpeed;

	uint32_t nType;
	uint32_t nId;

	bool bRemove = false;
};

class RayCasting : public def::GameEngine
{
public:
	RayCasting()
	{
		SetTitle("Ray Casting");
	}

	virtual ~RayCasting()
	{
		delete sprTiles;
		delete[] fDepthBuffer;
	}

private:
	enum Objects
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

	enum Side
	{
		FLOOR,
		CEILING,
		WALL
	};

	def::Sprite* sprTiles = nullptr;

	int nMapCellSize = 3;

	int nFloorId = Objects::GREYSTONE;
	int nCeilingId = Objects::WOOD;

	std::string sMap;

	def::vi2d vMapSize = { 32, 32 };
	def::vi2d vTexSize = { 64, 64 };

	def::vf2d vPlayerPos = { 7.0f, 3.0f };
	def::vf2d vPlayerVel = { -1.0, 0.0f };
	def::vf2d vPlayerPlane = { 0.0f, 0.66f };

	float fMoveSpeed = 5.0f;
	float fRotSpeed = 3.0f;
	float fDepth = 16.0f;

	std::vector<Object> vecObjects;
	float* fDepthBuffer = nullptr;

protected:
	bool OnUserCreate() override
	{
		sprTiles = new def::Sprite("tileset.png");

		sMap =
			"77777777777777777.........777777"
			"7..............................7"
			"7...8...............8..........7"
			"7...8...............8..........7"
			"7...88..............88.........7"
			"7....8...............8.........7"
			"7..............................7"
			"7..88..............88..........7"
			"7.......8888888.........88888887"
			"7..............................7"
			"...........8...............8...7"
			".........888.............888...7"
			"...............................2"
			"...............................7"
			"...............................7"
			"............88888..............7"
			"...............................7"
			"7..............................7"
			"7...6...............6..........7"
			"7...6...............6..........7"
			"7...66..............66.........7"
			"7....6...............6.........7"
			"7..............................7"
			"7..66..............66..........7"
			"7.......6666666.........66666667"
			"7..............................."
			"7..........6...............6...."
			"7........666.............666...."
			"7..............................."
			"7..............................."
			"7..............................."
			"777777..................77777777";

		vecObjects.push_back({ {8.5f, 8.5f}, {0.0f, 0.0f}, 0.0f, Objects::BARREL });
		vecObjects.push_back({ {7.5f, 7.5f}, {0.0f, 0.0f}, 0.0f, Objects::BARREL });
		vecObjects.push_back({ {10.0f, 3.0f}, {0.0f, 0.0f}, 0.0f, Objects::BARREL });

		fDepthBuffer = new float[ScreenWidth()];

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		// Remove redundant objects
		auto itRemove = std::remove_if(vecObjects.begin(), vecObjects.end(), [](const Object& o) { return o.bRemove; });
		if (itRemove != vecObjects.end()) vecObjects.erase(itRemove);

		if (GetKey(def::Key::W).bHeld)
		{
			def::vf2d vNewPlayerPos = vPlayerPos + vPlayerVel * fMoveSpeed * fDeltaTime;

			if (vNewPlayerPos >= def::vf2d(0, 0) && vNewPlayerPos < def::vf2d(vMapSize))
			{
				if (sMap[(int)vNewPlayerPos.y * vMapSize.x + vNewPlayerPos.x] == '.')
					vPlayerPos = vNewPlayerPos;
			}
		}

		if (GetKey(def::Key::S).bHeld)
		{
			def::vf2d vNewPlayerPos = vPlayerPos - vPlayerVel * fMoveSpeed * fDeltaTime;

			if (vNewPlayerPos >= def::vf2d(0, 0) && vNewPlayerPos < def::vf2d(vMapSize))
			{
				if (sMap[(int)vNewPlayerPos.y * vMapSize.x + vNewPlayerPos.x] == '.')
					vPlayerPos = vNewPlayerPos;
			}
		}

		if (GetKey(def::Key::A).bHeld)
		{
			float fOldVelX = vPlayerVel.x;
			float fOldPlaneX = vPlayerPlane.x;

			vPlayerVel.x = vPlayerVel.x * cos(fRotSpeed * fDeltaTime) - vPlayerVel.y * sin(fRotSpeed * fDeltaTime);
			vPlayerVel.y = fOldVelX * sin(fRotSpeed * fDeltaTime) + vPlayerVel.y * cos(fRotSpeed * fDeltaTime);

			vPlayerPlane.x = vPlayerPlane.x * cos(fRotSpeed * fDeltaTime) - vPlayerPlane.y * sin(fRotSpeed * fDeltaTime);
			vPlayerPlane.y = fOldPlaneX * sin(fRotSpeed * fDeltaTime) + vPlayerPlane.y * cos(fRotSpeed * fDeltaTime);
		}

		if (GetKey(def::Key::D).bHeld)
		{
			float fOldVelX = vPlayerVel.x;
			float fOldPlaneX = vPlayerPlane.x;

			vPlayerVel.x = vPlayerVel.x * cos(-fRotSpeed * fDeltaTime) - vPlayerVel.y * sin(-fRotSpeed * fDeltaTime);
			vPlayerVel.y = fOldVelX * sin(-fRotSpeed * fDeltaTime) + vPlayerVel.y * cos(-fRotSpeed * fDeltaTime);

			vPlayerPlane.x = vPlayerPlane.x * cos(-fRotSpeed * fDeltaTime) - vPlayerPlane.y * sin(-fRotSpeed * fDeltaTime);
			vPlayerPlane.y = fOldPlaneX * sin(-fRotSpeed * fDeltaTime) + vPlayerPlane.y * cos(-fRotSpeed * fDeltaTime);
		}

		// Check for collision
		for (auto& o1 : vecObjects)
		{
			for (auto& o2 : vecObjects)
			{
				if (o1.nId != o2.nId)
				{
					if (o1.vPos.round() == o2.vPos.round())
					{
						if (o1.nType == Objects::BULLET || o2.nType == Objects::BULLET)
						{
							o1.bRemove = true;
							o2.bRemove = true;
						}
					}
				}
			}
		}

		Clear(def::BLACK);

		// Perform DDA raycast algorithm
		for (int x = 0; x < ScreenWidth(); x++)
		{
			float fCameraX = 2.0f * (float)x / (float)ScreenWidth() - 1.0f;

			def::vf2d vRayDir = vPlayerVel + vPlayerPlane * fCameraX;
			def::vf2d vDistance = (1.0f / vRayDir).abs();

			bool bHitWall = false;
			bool bNoWall = false;
			int nSide = 0;

			def::vi2d vStep;
			def::vf2d vFromCurrentDistance;
			def::vi2d vMapPos = vPlayerPos;

			float fDistanceToWall;

			if (vRayDir.x < 0.0f)
			{
				vStep.x = -1;
				vFromCurrentDistance.x = (vPlayerPos.x - (float)vMapPos.x) * vDistance.x;
			}
			else
			{
				vStep.x = 1;
				vFromCurrentDistance.x = ((float)vMapPos.x + 1.0f - vPlayerPos.x) * vDistance.x;
			}

			if (vRayDir.y < 0.0f)
			{
				vStep.y = -1;
				vFromCurrentDistance.y = (vPlayerPos.y - (float)vMapPos.y) * vDistance.y;
			}
			else
			{
				vStep.y = 1;
				vFromCurrentDistance.y = ((float)vMapPos.y + 1.0f - vPlayerPos.y) * vDistance.y;
			}

			while (!bHitWall && !bNoWall)
			{
				if (vFromCurrentDistance.x < vFromCurrentDistance.y)
				{
					vFromCurrentDistance.x += vDistance.x;
					vMapPos.x += vStep.x;
					nSide = 0;
				}
				else
				{
					vFromCurrentDistance.y += vDistance.y;
					vMapPos.y += vStep.y;
					nSide = 1;
				}

				if (vMapPos.y < 0 || vMapPos.y >= vMapSize.y || vMapPos.x < 0 || vMapPos.x >= vMapSize.x)
				{
					fDistanceToWall = fDepth;
					bNoWall = true;
				}

				if (!bNoWall && std::isdigit(sMap[vMapPos.y * vMapSize.x + vMapPos.x]))
					bHitWall = true;
			}

			if (nSide == 0)
				fDistanceToWall = vFromCurrentDistance.x - vDistance.x;
			else
				fDistanceToWall = vFromCurrentDistance.y - vDistance.y;

			int nLineHeight = int((float)ScreenHeight() / fDistanceToWall) * 2;

			int nCeiling = -nLineHeight / 2 + ScreenHeight() / 2;
			int nFloor = nLineHeight / 2 + ScreenHeight() / 2;

			if (nCeiling < 0)
				nCeiling = 0;

			if (nFloor >= ScreenHeight())
				nFloor = ScreenHeight();

			float fTestPoint;
			float fTexStep;
			float fTexPos;

			if (nSide == 0)
				fTestPoint = vPlayerPos.y + vRayDir.y * fDistanceToWall;
			else
				fTestPoint = vPlayerPos.x + vRayDir.x * fDistanceToWall;

			fTestPoint -= floorf(fTestPoint);

			def::vi2d vTex = { int(fTestPoint * (float)vTexSize.x), 0 };

			if ((nSide == 0 && vRayDir.x > 0.0f) || (nSide == 1 && vRayDir.y < 0.0f))
				vTex.x = vTexSize.x - vTex.x - 1;

			fTexStep = (float)vTexSize.y / (float)nLineHeight;
			fTexPos = float(nCeiling - ScreenHeight() / 2 + nLineHeight / 2) * fTexStep;

			for (int y = 0; y <= nFloor; y++)
			{
				if (y <= nCeiling) // ceiling and floor
				{
					float fPlaneZ = float(ScreenHeight() / 2) / float(ScreenHeight() / 2 - y);

					def::vf2d vPlanePoint = vPlayerPos + 2.0f * vRayDir * fPlaneZ;
					def::vf2d vPlaneSample = vPlanePoint - vPlanePoint.floor();

					def::vi2d vTexPos = (vPlaneSample * def::vf2d(vTexSize)).min(vTexSize);

					Draw(x, y, sprTiles->GetPixel(nCeilingId * vTexSize.x + vTexPos.x, vTexPos.y)); // ceiling
					Draw(x, ScreenHeight() - y, sprTiles->GetPixel(nFloorId * vTexSize.x + vTexPos.x, vTexPos.y)); // floor
				}
				else if (y > nCeiling && !bNoWall) // wall
				{
					if (fDistanceToWall < fDepth)
					{
						vTex.y = (int)fTexPos & (vTexSize.y - 1);
						fTexPos += fTexStep;

						Draw(x, y, sprTiles->GetPixel(((int)sMap[vMapPos.y * vMapSize.x + vMapPos.x] - 48)* vTexSize.x + vTex.x, vTex.y));
					}
				}
			}

			fDepthBuffer[x] = fDistanceToWall;
		}

		// Update and draw textured objects
		for (auto& o : vecObjects)
		{
			o.vPos += o.vVel * o.fSpeed * fDeltaTime;

			if (o.vPos.floor() >= def::vf2d(0, 0) && o.vPos.floor() < def::vf2d(vMapSize) && !std::isdigit(sMap[(int)o.vPos.y * vMapSize.x + (int)o.vPos.x]))
			{
				def::vf2d vObjectPos = o.vPos - vPlayerPos;
				
				float fInvDet = 1.0f / (vPlayerPlane.x * vPlayerVel.y - vPlayerPlane.y * vPlayerVel.x);

				def::vf2d vTransform =
				{
					fInvDet * (vPlayerVel.y * vObjectPos.x - vPlayerVel.x * vObjectPos.y),
					fInvDet * (-vPlayerPlane.y * vObjectPos.x + vPlayerPlane.x * vObjectPos.y)
				};

				float fAspectRatio = vTransform.x / vTransform.y;

				def::vi2d vObjectScreenPos = { int(float(ScreenWidth() / 2) * (1.0f + fAspectRatio)), ScreenHeight() / 2 };
				def::vi2d vObjectScreenSize = (def::vf2d(ScreenHeight(), ScreenHeight()) / def::vf2d(vTransform.y, vTransform.y)).abs();

				def::vi2d vCeiling = vObjectScreenSize / -2 + vObjectScreenPos;
				def::vi2d vFloor = vObjectScreenSize / 2 + vObjectScreenPos;

				vCeiling = vCeiling.max(def::vi2d(0, 0)).min(ScreenSize());
				vFloor = vFloor.max(def::vi2d(0, 0)).min(ScreenSize());

				SetPixelMode(def::Pixel::MASK);

				for (int x = vCeiling.x; x < vFloor.x; x++)
				{
					int nTexX = (256 * (x - (-vObjectScreenSize.x / 2 + vObjectScreenPos.x)) * vTexSize.x / vObjectScreenSize.x) / 256;

					if (vTransform.y >= 0 && x >= 0 && x < ScreenWidth() && vTransform.y < fDepthBuffer[x])
					{
						for (int y = vCeiling.y; y < vFloor.y; y++)
						{
							int d = y * 256 - ScreenHeight() * 128 + vObjectScreenSize.y * 128;
							int nTexY = (d * vTexSize.y / vObjectScreenSize.y) / 256;

							Draw(x, y, sprTiles->GetPixel(o.nType * vTexSize.x + nTexX, nTexY));
							fDepthBuffer[x] = vTransform.y;
						}
					}
				}

				SetPixelMode(def::Pixel::DEFAULT);
			}
			else
			{
				o.bRemove = true;
			}
		}

		// Draw map
		for (int x = 0; x < vMapSize.x; x++)
			for (int y = 0; y < vMapSize.y; y++)
			{
				if (sMap[y * vMapSize.x + x] == '.')
					FillRectangle(x * 2, y * 2, 2, 2, def::GREY);
				else
					FillRectangle(x * 2, y * 2, 2, 2, def::WHITE);
			}
		FillRectangle((int)vPlayerPos.x * 2, (int)vPlayerPos.y * 2, 2, 2, def::YELLOW);

		if (GetMouse(0).bPressed)
		{
			Object o;
			o.vPos = vPlayerPos;
			o.vVel = vPlayerVel;
			o.fSpeed = 5.0f;
			o.nType = Objects::BULLET;
			o.nId = vecObjects.size();
			vecObjects.push_back(o);
		}

		return true;
	}

};

int main()
{
	RayCasting demo;

	if (demo.Construct(512, 480, 2, 2))
		demo.Run();

	return 0;
}
