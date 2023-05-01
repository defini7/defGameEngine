#include "defGameEngine.h"

using namespace std;
using namespace def;

class Mario : public def::GameEngine
{
public:
	Mario()
	{
		SetTitle("Mario");
		ShowFPS();
	}

private:
	vi2d vMapSize = { 16, 8 };
	vi2d vTileSize = { 32, 32 };

	string sMap;

	vf2d vPlayerPos = { 1.0f, 1.0f };
	vf2d vPlayerVel = { 0.0f, 0.0f };

	bool bPlayerOnGround = false;

	def::Sprite* sprMario = nullptr;
	def::Sprite* sprBackground = nullptr;

	int nGraphicID = 0;
	int nScore = 0;

protected:
	char GetTile(const vi2d& p)
	{
		if (p.x >= 0 && p.y >= 0 && p.x < vMapSize.x && p.y < vMapSize.y)
			return sMap[p.y * vMapSize.x + p.x];

		return ' ';
	}

	void SetTile(const vi2d& p, char c)
	{
		if (p.x >= 0 && p.y >= 0 && p.x < vMapSize.x && p.y < vMapSize.y)
			sMap[p.y * vMapSize.x + p.x] = c;
	}

	virtual bool OnUserCreate() override
	{
		sMap += "........xxxxxxxx";
		sMap += ".....ooo........";
		sMap += "................";
		sMap += "..xxxxxxxx...xxx";
		sMap += "..########...###";
		sMap += "..########..x###";
		sMap += "................";
		sMap += "xxxxxxxxxxxxxxxx";

		sprMario = new def::Sprite("mario.png");
		sprBackground = new def::Sprite("sky.png");

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

		if (GetTile(vNewPlayerPos) == 'o')
		{
			SetTile(vNewPlayerPos, '.');
			nScore++;
		}

		if (GetTile(vNewPlayerPos + vf2d(0.0f, 1.0f)) == 'o')
		{
			SetTile(vNewPlayerPos + vf2d(0.0f, 1.0f), '.');
			nScore++;
		}

		if (GetTile(vNewPlayerPos + vf2d(1.0f, 0.0f)) == 'o')
		{
			SetTile(vNewPlayerPos + vf2d(1.0f, 0.0f), '.');
			nScore++;
		}

		if (GetTile(vNewPlayerPos + vf2d(1.0f, 1.0f)) == 'o')
		{
			SetTile(vNewPlayerPos + vf2d(1.0f, 1.0f), '.');
			nScore++;
		}

		if (vPlayerVel.x <= 0.0f)
		{
			if (GetTile(vi2d(vNewPlayerPos.x + 0.0f, vPlayerPos.y + 0.0f)) != '.' || GetTile(vi2d(vNewPlayerPos.x + 0.0f, vPlayerPos.y + 0.9f)) != '.')
			{
				vNewPlayerPos.x = (float)((int)vNewPlayerPos.x) + 1.0f;
				vPlayerVel.x = 0.0f;
			}
		}
		else
		{
			if (GetTile(vi2d(vNewPlayerPos.x + 1.0f, vPlayerPos.y + 0.0f)) != '.' || GetTile(vi2d(vNewPlayerPos.x + 1.0f, vPlayerPos.y + 0.9f)) != '.')
			{
				vNewPlayerPos.x = (float)((int)vNewPlayerPos.x);
				vPlayerVel.x = 0.0f;
			}
		}

		bPlayerOnGround = false;
		if (vPlayerVel.y <= 0.0f)
		{
			if (GetTile(vNewPlayerPos) != '.' || GetTile(vNewPlayerPos + vf2d(0.9f, 0.0f)) != '.')
			{
				vNewPlayerPos.y = (float)((int)vNewPlayerPos.y) + 1.0f;
				vPlayerVel.y = 0.0f;
			}
		}
		else
		{
			if (GetTile(vNewPlayerPos + vf2d(0.0f, 1.0f)) != '.' || GetTile(vNewPlayerPos + vf2d(0.9f, 1.0f)) != '.')
			{
				vNewPlayerPos.y = (float)((int)vNewPlayerPos.y);
				vPlayerVel.y = 0.0f;
				bPlayerOnGround = true;
			}
		}

		vPlayerPos = vNewPlayerPos;

		DrawSprite(0, 0, sprBackground);

		vi2d vVisibleTiles = GetScreenSize() / vTileSize;
		vf2d vOffset = vPlayerPos - (vf2d)vVisibleTiles * 0.5f;

		vOffset = vOffset.max({ 0.0f, 0.0f });
		vOffset = vOffset.min(vMapSize - vVisibleTiles);

		vf2d vTileOffset = (vOffset - (vf2d)(vi2d)vOffset) * (vf2d)vTileSize;

		vi2d vTile;
		for (vTile.y = -1; vTile.y < vVisibleTiles.y + 1; vTile.y++)
		{
			for (vTile.x = -1; vTile.x < vVisibleTiles.x + 1; vTile.x++)
			{
				char tile = GetTile(vTile + (vi2d)vOffset);

				vi2d p = vf2d(vTile * vTileSize) - vTileOffset;

				switch (tile)
				{
				case ' ': FillRectangle(p, vTileSize, def::BLACK); break;
				case 'o': DrawPartialSprite(p, { 4 * vTileSize.x, 0 }, vTileSize, sprMario); break;
				case 'x': DrawPartialSprite(p, { 5 * vTileSize.x, 0 }, vTileSize, sprMario); break;
				case '#': DrawPartialSprite(p, { 6 * vTileSize.x, 0 }, vTileSize, sprMario); break;
				}
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
