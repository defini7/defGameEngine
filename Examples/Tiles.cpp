#include "defGameEngine.h"

using namespace std;

class Mario : public def::GameEngine
{
public:
	Mario()
	{
		SetTitle("Mario");
	}

private:
	int nMapWidth = 16;
	int nMapHeight = 8;

	int nTileWidth = 32;
	int nTileHeight = 32;

	string sMap;

	float fPlayerX = 1.0f;
	float fPlayerY = 1.0f;

	float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;

	float fVelocityX = 0.0f;
	float fVelocityY = 0.0f;

	bool bPlayerOnGround = false;

	def::Sprite* sprMario = nullptr;

	int nGraphicID = 0;

protected:
	char GetTile(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < nMapWidth && y < nMapHeight)
			return sMap[y * nMapWidth + x];

		return ' ';
	}

	void SetTile(int x, int y, char c)
	{
		if (x >= 0 && y >= 0 && x < nMapWidth && y < nMapHeight)
			sMap[y * nMapWidth + x] = c;
	}

	virtual bool OnUserCreate() override
	{
		sMap += "........xxxxxxxx";
		sMap += ".....ooo........";
		sMap += "................";
		sMap += "..xxxxxxxx...xxx";
		sMap += ".........x...x..";
		sMap += ".........x..xx..";
		sMap += "................";
		sMap += "xxxxxxxxxxxxxxxx";

		sprMario = new def::Sprite("mario.png");

		return true;
	}

	virtual bool OnUserUpdate(float fDeltaTime) override
	{
		if (GetWindowState() & def::WS_FOCUSED)
		{
			if (GetKey(def::Key::UP).bHeld)
			{
				fVelocityY = -6.0f;
				nGraphicID = (fVelocityX >= 0.0f) ? 2 : 3;
			}

			if (GetKey(def::Key::DOWN).bHeld)
				fVelocityY = 6.0f;

			if (GetKey(def::Key::LEFT).bHeld)
			{
				fVelocityX += (bPlayerOnGround ? -25.0f : -15.0f) * fDeltaTime;
				if (bPlayerOnGround) nGraphicID = 1;
			}

			if (GetKey(def::Key::RIGHT).bHeld)
			{
				fVelocityX += (bPlayerOnGround ? 25.0f : 15.0f) * fDeltaTime;
				if (bPlayerOnGround) nGraphicID = 0;
			}

			if (GetKey(def::Key::SPACE).bPressed)
			{
				if (fVelocityY == 0.0f)
					fVelocityY = -12.0f;

				nGraphicID = (fVelocityX >= 0.0f) ? 2 : 3;
			}
		}

		fVelocityY += 20.0f * fDeltaTime;

		if (bPlayerOnGround)
		{
			fVelocityX += -3.0f * fVelocityX * fDeltaTime;
			if (fabs(fVelocityX) < 0.01f) fVelocityX = 0.0f;
		}

		if (fVelocityX > 10.0f)
			fVelocityX = 10.0f;

		if (fVelocityX < -10.0f)
			fVelocityX = -10.0f;

		if (fVelocityY > 100.0f)
			fVelocityY = 100.0f;

		if (fVelocityY < -100.0f)
			fVelocityY = -100.0f;

		float fNewPlayerX = fPlayerX + fVelocityX * fDeltaTime;
		float fNewPlayerY = fPlayerY + fVelocityY * fDeltaTime;

		if (GetTile(fNewPlayerX + 0.0f, fNewPlayerY + 0.0f) == 'o')
			SetTile(fNewPlayerX + 0.0f, fNewPlayerY + 0.0f, '.');

		if (GetTile(fNewPlayerX + 0.0f, fNewPlayerY + 1.0f) == 'o')
			SetTile(fNewPlayerX + 0.0f, fNewPlayerY + 1.0f, '.');

		if (GetTile(fNewPlayerX + 1.0f, fNewPlayerY + 0.0f) == 'o')
			SetTile(fNewPlayerX + 1.0f, fNewPlayerY + 0.0f, '.');

		if (GetTile(fNewPlayerX + 1.0f, fNewPlayerY + 1.0f) == 'o')
			SetTile(fNewPlayerX + 1.0f, fNewPlayerY + 1.0f, '.');

		if (fVelocityX <= 0.0f)
		{
			if (GetTile(fNewPlayerX + 0.0f, fPlayerY + 0.0f) != '.' || GetTile(fNewPlayerX + 0.0f, fPlayerY + 0.9f) != '.')
			{
				fNewPlayerX = (int)fNewPlayerX + 1.0f;
				fVelocityX = 0.0f;
			}
		}
		else
		{
			if (GetTile(fNewPlayerX + 1.0f, fPlayerY + 0.0f) != '.' || GetTile(fNewPlayerX + 1.0f, fPlayerY + 0.9f) != '.')
			{
				fNewPlayerX = (int)fNewPlayerX;
				fVelocityX = 0.0f;
			}
		}

		bPlayerOnGround = false;
		if (fVelocityY <= 0.0f)
		{
			if (GetTile(fNewPlayerX + 0.0f, fNewPlayerY + 0.0f) != '.' || GetTile(fNewPlayerX + 0.9f, fNewPlayerY + 0.0f) != '.')
			{
				fNewPlayerY = (int)fNewPlayerY + 1.0f;
				fVelocityY = 0.0f;
			}
		}
		else
		{
			if (GetTile(fNewPlayerX + 0.0f, fNewPlayerY + 1.0f) != '.' || GetTile(fNewPlayerX + 0.9f, fNewPlayerY + 1.0f) != '.')
			{
				fNewPlayerY = (int)fNewPlayerY;
				fVelocityY = 0.0f;
				bPlayerOnGround = true;
			}
		}

		fPlayerX = fNewPlayerX;
		fPlayerY = fNewPlayerY;

		int nVisibleTilesX = GetScreenWidth() / nTileWidth;
		int nVisibleTilesY = GetScreenHeight() / nTileHeight;

		float fOffsetX = fPlayerX - (float)nVisibleTilesX * 0.5f;
		float fOffsetY = fPlayerY - (float)nVisibleTilesY * 0.5f;

		if (fOffsetX < 0.0f) fOffsetX = 0.0f;
		if (fOffsetY < 0.0f) fOffsetY = 0.0f;

		if (fOffsetX > nMapWidth - nVisibleTilesX)  fOffsetX = (float)(nMapWidth - nVisibleTilesX);
		if (fOffsetY > nMapHeight - nVisibleTilesY) fOffsetY = (float)(nMapHeight - nVisibleTilesY);

		float fTileOffsetX = (fOffsetX - (int)fOffsetX) * (float)nTileWidth;
		float fTileOffsetY = (fOffsetY - (int)fOffsetY) * (float)nTileHeight;

		for (int j = -1; j < nVisibleTilesY + 1; j++)
		{
			for (int i = -1; i < nVisibleTilesX + 1; i++)
			{
				char tile = GetTile(i + (int)fOffsetX, j + (int)fOffsetY);

				int x = (int)((float)(i * nTileWidth) - fTileOffsetX);
				int y = (int)((float)(j * nTileHeight) - fTileOffsetY);

				switch (tile)
				{
				case '.': FillRectangle(x, y, nTileWidth, nTileHeight, def::CYAN);	break;
				case 'x': FillRectangle(x, y, nTileWidth, nTileHeight, def::RED);	break;
				case ' ': FillRectangle(x, y, nTileWidth, nTileHeight, def::BLACK); break;
				case 'o':
					FillRectangle(x, y, nTileWidth, nTileHeight, def::CYAN);
					DrawPartialSprite(x, y, 4 * nTileWidth, 0, nTileWidth, nTileHeight, sprMario);
					break;
				}
			}
		}

		int nPlayerX = (int)((fPlayerX - fOffsetX) * (float)nTileWidth);
		int nPlayerY = (int)((fPlayerY - fOffsetY) * (float)nTileHeight);

		//FillRectangle(nPlayerX, nPlayerY, nTileWidth, nTileHeight, def::DARK_BLUE);
		DrawPartialSprite(nPlayerX, nPlayerY, nTileWidth * nGraphicID, 0, nTileWidth, nTileHeight, sprMario);

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
