#include "defGameEngine.h"

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

	bool RectVsRect(def::vf2d& vPos1, def::vf2d& vSize1, def::vf2d& vPos2, def::vf2d& vSize2)
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

		bAlive = (vPosition >= def::vf2d(0.0f, 0.0f) && vPosition < GetScreenSize<float>());

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

		FillRectangle(def::vi2d(vPosition), vSize, def::ORANGE);

		int nSection = 0;
		for (auto s : listSections)
		{
			if (s != 0)
			{
				int nBottomX = int((float)nSection * fSectionWidth + 10.0f - fLevelPosition);
				int nBottomY = GetScreenHeight() - s + 20;

				int nBottomSizeX = int((float)nSection * fSectionWidth + 15.0f - fLevelPosition) - nBottomX + 20;
				int nBottomSizeY = GetScreenHeight() - nBottomY;

				int nTopX = int((float)nSection * fSectionWidth + 10.0f - fLevelPosition);
				int nTopY = 0;

				int nTopSizeX = int((float)nSection * fSectionWidth + 15.0f - fLevelPosition) - nTopX + 20;
				int nTopSizeY = (GetScreenHeight() - s - 15) - nTopY - 20;

				FillRectangle(nTopX, nTopY, nTopSizeX, nTopSizeY, def::GREEN);
				FillRectangle(nBottomX, nBottomY, nBottomSizeX, nBottomSizeY, def::GREEN);

				bool bCollideTop = RectVsRect(
					def::vf2d(nTopX, nTopY).ref(), def::vf2d(nTopSizeX, nTopSizeY).ref(),
					vPosition, def::vf2d(vSize).ref()
				);

				bool bCollideBottom = RectVsRect(
					def::vf2d(nBottomX, nBottomY).ref(), def::vf2d(nBottomSizeX, nBottomSizeY).ref(),
					vPosition, def::vf2d(vSize).ref()
				);

				if (bCollideTop || bCollideBottom)
					bAlive = false;
			}

			nSection++;
		}

		DrawString(10, 10, "Score: " + std::to_string(nScore), def::WHITE);

		return true;
	}

private:
	float fGravity;

	def::vf2d vPosition;
	def::vi2d vSize;

	def::vf2d vVelocity;
	def::vf2d vAcceleration;

	float fSectionWidth;
	std::list<int> listSections;
	float fLevelPosition;

	int nObstacleCount;
	int nMaxObstacleCount;

	bool bAlive;
	int nScore;

};

int main()
{
	Sample demo;

	def::rcode err = demo.Construct(800, 256, 2, 2);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
