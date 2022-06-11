#include "defGameEngine.h"

enum CellType
{
	BLANK,
	GRASS
};

struct sWarm
{
	def::vf2d vPos;
	def::vi2d vSize;

	def::Pixel pTeam;

	float fSpeed;
	float fAcceleration;
};

struct sDebris
{
	def::vf2d vPos;
	def::vf2d vVelocity;
	def::vf2d vStart;

	float fTime;
};

struct sGunPart
{
	def::vf2d vPos;
	def::vi2d vSize;

	float fAngle;
};

struct sGun
{
	sGunPart gpBarrel;
	sGunPart gpStand;

	def::Pixel pTeam;
};

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Sample");
	}

private:
	void GenerateGround(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutput)
	{
		for (int x = 0; x < nCount; x++)
		{
			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleAccumulator = 0.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				int nPitch = nCount >> o;
				int nSample1 = (x / nPitch) * nPitch;
				int nSample2 = (nSample1 + nPitch) % nCount;

				float fBlend = (float)(x - nSample1) / (float)nPitch;
				float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];

				fNoise += fSample * fScale;
				fScaleAccumulator += fScale;
				fScale /= fBias;
			}

			fOutput[x] = fNoise / fScaleAccumulator;
		}
	}

	float* fGround = nullptr;
	float* fSeed = nullptr;

	int nOctaveCount = 4;
	float fScalingBias = 0.4f;

	float fOffsetX = 0;

	int nHalfHeight;

	CellType* pMap = nullptr;

	int nMapWidth;
	int nMapHeight;

	std::list<sWarm> listWarms;
	std::list<sDebris> listDebris;
	std::list<sGun> listGuns;

	def::Pixel pTeamToPopulate;

	def::Sprite* sprWarmRed = nullptr;
	def::Sprite* sprWarmGreen = nullptr;
	def::Sprite* sprWarmBlue = nullptr;

	def::Sprite* sprGunBarrel = nullptr;
	def::Sprite* sprGunStand = nullptr;

	uint32_t nRedScore = 0;
	uint32_t nGreenScore = 0;
	uint32_t nBlueScore = 0;

protected:
	void CreateWarm(def::Pixel pTeam, float fPosX, float fPosY, int nWidth, int nHeight)
	{
		sWarm w;

		w.pTeam = pTeam;

		w.vPos = def::vf2d(fPosX, fPosY);
		w.vSize = def::vi2d(nWidth, nHeight);

		w.fAcceleration = 0.0f;
		w.fSpeed = 2.0f;

		listWarms.push_back(w);
	}

	void CreateDebris(float fPosX, float fPosY, float fVelX, float fVelY)
	{
		sDebris d;

		d.vPos = def::vf2d(fPosX, fPosY);
		d.vStart = d.vPos;
		d.vVelocity = def::vf2d(fVelX, fVelY);
		d.fTime = 0.0f;

		listDebris.push_back(d);
	}

	void CreateGun(def::Pixel pTeam, float fPosX, float fPosY, int nBarrelWidth, int nBarrelHeight, int nStandWidth, int nStandHeight)
	{
		sGun g;

		g.gpBarrel.vPos = def::vf2d(fPosX, fPosY);
		g.gpBarrel.vSize = def::vi2d(nBarrelWidth, nBarrelHeight);
		g.gpBarrel.fAngle = 0.0f;

		g.gpStand.vPos = def::vf2d(fPosX, fPosY + nBarrelHeight);
		g.gpStand.vSize = def::vi2d(nStandWidth, nStandHeight);

		g.pTeam = pTeam;

		listGuns.push_back(g);
	}

	void GenerateSeed()
	{
		for (int i = 0; i < GetScreenWidth() * 2; i++) 
			fSeed[i] = (float)rand() / (float)RAND_MAX;
	}

	void MakeBoom()
	{
		int x = GetMouseX() + (int)fOffsetX;
		int y = GetMouseY();

		if (pMap[y * nMapWidth + x] != BLANK)
		{
			const int nExplosionRadius = 20;

			int x1 = 0;
			int y1 = nExplosionRadius;
			int p1 = 3 - 2 * nExplosionRadius;

			auto make_ray = [&](int sx, int ex, int ny)
			{
				for (int i = sx; i <= ex; i++)
					if (ny < nMapHeight && i < nMapWidth && ny >= 0 && i >= 0)
						pMap[ny * nMapWidth + i] = BLANK;

				if (rand() % 10 == 0)
					CreateDebris((float)x, (float)y, ex * (rand() % 2 == 0 ? 1.0f : -1.0f), ny * (rand() % 2 == 0 ? 1.0f : -1.0f));
			};

			while (y1 >= x1)
			{
				make_ray(x - x1, x + x1, y - y1);
				make_ray(x - y1, x + y1, y - x1);
				make_ray(x - x1, x + x1, y + y1);
				make_ray(x - y1, x + y1, y + x1);

				if (p1 < 0)
					p1 += 4 * x1++ + 6;
				else
					p1 += 4 * (x1++ - y1--) + 10;
			}
		}
	}

	void AddWarm()
	{
		bool bCanCreate = true;

		for (int i = 0; i < 7; i++) // 7 - width of sprite
			for (int j = 0; j < 16; j++) // 16 - height of sprite
				if (pMap[(GetMouseY() + j) * nMapWidth + (GetMouseX() + i)] != BLANK)
					bCanCreate = false;

		if (bCanCreate)
			CreateWarm(pTeamToPopulate, GetMouseX(), GetMouseY(), 8, 16);
	}

	void AddGun()
	{
		bool bCanCreate = true;

		for (int i = 0; i < 7; i++) // 7 - width of sprite
			for (int j = 0; j < 16; j++) // 16 - height of sprite
				if (pMap[(GetMouseY() + j) * nMapWidth + (GetMouseX() + i)] != BLANK)
					bCanCreate = false;

		if (bCanCreate)
			CreateGun(pTeamToPopulate, GetMouseX(), GetMouseY(), sprGunBarrel->GetWidth(), sprGunBarrel->GetHeight(), sprGunStand->GetWidth(), sprGunStand->GetHeight());
	}

	void DrawGround()
	{
		for (int x = (int)fOffsetX; x < GetScreenWidth() + (int)fOffsetX; x++)
			for (int y = 0; y < GetScreenHeight(); y++)
			{
				CellType cell = pMap[y * GetScreenWidth() * 2 + x];

				if (cell != BLANK)
					Draw(x - (int)fOffsetX, y, def::GREEN);
			}
	}

	void RemoveUnusedWarms()
	{
		auto itUnusedWarms = std::remove_if(listWarms.begin(), listWarms.end(), [&](sWarm& w) { return w.vPos.y + w.vSize.y > GetScreenHeight(); });

		if (itUnusedWarms != listWarms.end())
			listWarms.erase(itUnusedWarms);
	}

	void DrawWarms(float fDifferenceOffsetX)
	{
		for (auto& w : listWarms)
		{
			if (w.vPos.x + w.vSize.x > 0 && w.vPos.x < GetScreenWidth() + (int)fOffsetX)
			{
				if (w.pTeam == def::RED)
					DrawSprite((int)w.vPos.x, (int)w.vPos.y, sprWarmRed);
				else if (w.pTeam == def::GREEN)
					DrawSprite((int)w.vPos.x, (int)w.vPos.y, sprWarmGreen);
				else if (w.pTeam == def::BLUE)
					DrawSprite((int)w.vPos.x, (int)w.vPos.y, sprWarmBlue);

				int nUnderY = w.vPos.y + w.vSize.y + 1;

				bool bFloor = false;

				for (int i = 0; i < w.vSize.x; i++)
					if (nUnderY < nMapHeight)
						if (pMap[nUnderY * nMapWidth + int(w.vPos.x + fOffsetX) + i] != BLANK)
							bFloor = true;

				if (!bFloor)
				{
					w.fAcceleration += GetDeltaTime();

					if (w.fAcceleration > 1.0f)
						w.fAcceleration = 1.0f;

					w.vPos.y += w.fAcceleration * w.fSpeed;
				}
				else
					w.fAcceleration = 0.0f;
			}

			w.vPos.x += fDifferenceOffsetX;
		}
	}

	void RemoveUnusedDebris()
	{
		auto itUnusedDebris = std::remove_if(listDebris.begin(), listDebris.end(), [&](sDebris& d)
			{
				return d.fTime > 1.0f || d.vPos < def::vf2d(0.0f, 0.0f) || d.vPos > def::vf2d((float)nMapWidth, (float)nMapHeight);
			});

		if (itUnusedDebris != listDebris.end())
			listDebris.erase(itUnusedDebris);
	}

	void DrawDebris()
	{
		for (auto& d : listDebris)
		{
			Draw((int)d.vPos.x, (int)d.vPos.y, def::GREEN);

			d.vPos.x += d.vVelocity.x * GetDeltaTime();
			d.vPos.y += d.vVelocity.y * GetDeltaTime();

			d.vVelocity.x *= (rand() % 5 ? 1.0f : -1.0f);
			d.vVelocity.y *= (rand() % 5 ? 1.0f : -1.0f);

			d.fTime += GetDeltaTime();
		}
	}

	void DrawScore()
	{
		DrawString(10, 10, std::to_string(nRedScore), def::RED);
		DrawString(10, 20, std::to_string(nGreenScore), def::GREEN);
		DrawString(10, 30, std::to_string(nBlueScore), def::BLUE);
	}

	bool OnUserCreate() override
	{
		srand(time(nullptr));

		sprWarmRed = CreateSprite("warms/warm_red.png");
		sprWarmGreen = CreateSprite("warms/warm_green.png");
		sprWarmBlue = CreateSprite("warms/warm_blue.png");
		
		sprGunBarrel = CreateSprite("guns/gun_barrel.png");
		sprGunStand = CreateSprite("guns/gun_stand.png");

		pTeamToPopulate = def::GREEN;

		fGround = new float[GetScreenWidth() * 2];
		fSeed = new float[GetScreenWidth() * 2];

		nHalfHeight = GetScreenHeight() / 2;

		nMapWidth = GetScreenWidth() * 2;
		nMapHeight = GetScreenHeight();

		pMap = new CellType[nMapWidth * nMapHeight];
		memset(pMap, BLANK, sizeof(CellType) * nMapWidth * nMapHeight);

		GenerateSeed();
		GenerateGround(nMapWidth, fSeed, nOctaveCount, fScalingBias, fGround);

		for (int x = 0; x < nMapWidth; x++)
		{
			int y = int(-(fGround[x] * (float)GetScreenHeight() / 2.0f) + (float)nHalfHeight);
			
			for (int j = y + nHalfHeight / 2; j < GetScreenHeight(); j++)
				pMap[j * nMapWidth + x] = GRASS;
		}

		delete[] fGround;
		delete[] fSeed;

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		Clear(def::CYAN);

		float fDifferenceOffsetX = 0.0f;

		if (GetKey(def::Key::LEFT).bHeld)
		{
			fDifferenceOffsetX = fOffsetX;
			fOffsetX -= 20.0f * fDeltaTime;
			fDifferenceOffsetX -= fOffsetX;
		}

		if (GetKey(def::Key::RIGHT).bHeld)
		{
			fDifferenceOffsetX = fOffsetX;
			fOffsetX += 20.0f * fDeltaTime;
			fDifferenceOffsetX -= fOffsetX;
		}

		if ((int)fOffsetX > GetScreenWidth())
		{
			fDifferenceOffsetX = 0.0f;
			fOffsetX = (float)GetScreenWidth();
		}

		if (fOffsetX < 0.0f)
		{
			fDifferenceOffsetX = 0.0f;
			fOffsetX = 0.0f;
		}

		if (GetKey(def::Key::F1).bPressed)
			pTeamToPopulate = def::RED;

		if (GetKey(def::Key::F2).bPressed)
			pTeamToPopulate = def::GREEN;

		if (GetKey(def::Key::F3).bPressed)
			pTeamToPopulate = def::BLUE;

		if (GetMouse(1).bPressed)
			AddWarm();

		if (GetMouse(0).bPressed)
			MakeBoom();

		if (GetMouse(0).bPressed && GetKey(def::Key::G).bPressed)
			AddGun();

		DrawGround();
		DrawWarms(fDifferenceOffsetX);
		DrawDebris();
		DrawScore();

		RemoveUnusedWarms();
		RemoveUnusedDebris();

		return true;
	}

	void OnUserDestroy() override
	{
		delete[] pMap;
	}

};

int main()
{
	Sample demo;
	def::rcode err = demo.Construct(256, 240, 4, 4);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
