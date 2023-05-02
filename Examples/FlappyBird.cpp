#include "defGameEngine.h"

using namespace std;
using namespace def;

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Flappy Bird");
	}

protected:
	bool OnUserCreate() override
	{
		listSections = { 0, 0, 0, 0 };
		fSectionWidth = (float)GetScreenWidth() / float(listSections.size() - 1);

		fLevelPosition = 0.0f;

		nObstacleCount = 0;
		nMaxObstacleCount = 4;

		vPosition.x = 5;
		vPosition.y = GetScreenHeight() / 2;

		vAcceleration.x = 0.0f;
		vAcceleration.y = 0.0f;

		vVelocity.x = 0.0f;
		vVelocity.y = fGravity;

		vSize.x = 16;
		vSize.y = 16;

		fGravity = 50.0f;

		nScore = 0;
		bAlive = true;

		return true;
	}

	bool RectVsRect(const def::vf2d& vPos1, const def::vf2d& vSize1, const def::vf2d& vPos2, const def::vf2d& vSize2)
	{
		return vPos1 < vPos2 + vSize2 && vPos1 + vSize1 + 1 > vPos2;
	}

	void ResetGame()
	{
		listSections = { 0, 0, 0, 0 };
		fSectionWidth = (float)GetScreenWidth() / float(listSections.size() - 1);

		fLevelPosition = 0.0f;

		nObstacleCount = 0;
		nMaxObstacleCount = 4;

		vPosition.x = 5;
		vPosition.y = GetScreenHeight() / 2;

		vAcceleration.x = 0.0f;
		vAcceleration.y = 0.0f;

		vVelocity.x = 0.0f;
		vVelocity.y = fGravity;

		vSize.x = 16;
		vSize.y = 16;

		fGravity = 50.0f;

		nScore = 0;
		bAlive = true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (!bAlive)
		{
			if (GetKey(def::Key::SPACE).bPressed)
				ResetGame();

			DrawString(GetScreenWidth() / 2 - GetScreenWidth() / 5, GetScreenHeight() / 2, "\t\tGame Over!\nPress Space to try again!", def::WHITE);
			return true;
		}

		Clear(def::BLACK);

		bAlive = (vPosition >= vf2d(0.0f, 0.0f) && vPosition < (vf2d)GetScreenSize());

		if (GetKey(def::Key::SPACE).bPressed)
		{
			vAcceleration.y = 0.0f;
			vVelocity.y = -fGravity;
		}
		else
			vAcceleration.y += fGravity * fDeltaTime;

		if (vAcceleration.y >= fGravity)
			vAcceleration.y = fGravity;

		vVelocity += vAcceleration * fDeltaTime;
		vPosition += vVelocity * fDeltaTime;

		fLevelPosition += fGravity * fDeltaTime;

		if (fLevelPosition > fSectionWidth)
		{
			fLevelPosition -= fSectionWidth;
			listSections.pop_front();

			int x = rand() % (GetScreenHeight() - 20);

			if (x <= 0) x = 0;
			else nScore++;

			listSections.push_back(x);
		}

		FillRectangle(vPosition, vSize, def::ORANGE);

		int nSection = 0;
		for (auto s : listSections)
		{
			if (s != 0)
			{
				vi2d vBottom = { int((float)nSection * fSectionWidth + 10.0f - fLevelPosition), GetScreenHeight() - s + 20 };
				vi2d vBottomSize = { int((float)nSection * fSectionWidth + 15.0f - fLevelPosition) - vBottom.x + 20, GetScreenHeight() - vBottom.y };
				vi2d vTop = { int((float)nSection * fSectionWidth + 10.0f - fLevelPosition), 0 };
				vi2d vTopSize = { int((float)nSection * fSectionWidth + 15.0f - fLevelPosition) - vTop.x + 20, GetScreenHeight() - s - vTop.y - 35 };

				FillRectangle(vTop, vTopSize, def::GREEN);
				FillRectangle(vBottom, vBottomSize, def::GREEN);

				bool bCollideTop = RectVsRect(vTop, vTopSize, vPosition, vSize);
				bool bCollideBottom = RectVsRect(vBottom, vBottomSize, vPosition, vSize);

				bAlive = !(bCollideTop || bCollideBottom);
			}

			nSection++;
		}

		DrawString(10, 10, "Score: " + to_string(nScore), def::WHITE);

		return true;
	}

private:
	float fGravity;

	vf2d vPosition;
	vi2d vSize;

	vf2d vVelocity;
	vf2d vAcceleration;

	float fSectionWidth;
	list<int> listSections;
	float fLevelPosition;

	int nObstacleCount;
	int nMaxObstacleCount;

	bool bAlive;
	int nScore;

};

int main()
{
	Sample demo;

	if (demo.Construct(800, 256, 2, 2))
		demo.Run();

	return 0;
}
