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

		vPosition.x = 5;
		vPosition.y = GetScreenHeight() / 2;

		vAcceleration.x = 0.0f;
		vAcceleration.y = 0.0f;

		vVelocity.x = 0.0f;
		vVelocity.y = fGravity;

		vSize.x = 16;
		vSize.y = 16;

		return true;
	}

	bool RectVsRect(const def::vf2d& vPos1, const def::vf2d& vSize1, const def::vf2d& vPos2, const def::vf2d& vSize2)
	{
		return vPos1 - 1 < vPos2 + vSize2 && vPos1 + vSize1 + 1 > vPos2;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (!bAlive)
		{
			DrawString(GetScreenWidth() / 2 - GetScreenWidth() / 10, GetScreenHeight() / 2, "Game Over!", def::WHITE);
			return true;
		}

		Clear(def::BLACK);

		bAlive = (vPosition >= 0.0f && vPosition < GetScreenSize<float>());

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

		FillRectangle(vPosition.to<int>(), vSize, def::ORANGE);

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
					def::vf2d(nTopX, nTopY), def::vf2d(nTopSizeX, nTopSizeY),
					vPosition, vSize.to<float>()
				);

				bool bCollideBottom = RectVsRect(
					def::vf2d(nBottomX, nBottomY), def::vf2d(nBottomSizeX, nBottomSizeY),
					vPosition, vSize.to<float>()
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
	float fGravity = 50.0f;

	def::vf2d vPosition;
	def::vi2d vSize;

	def::vf2d vVelocity;
	def::vf2d vAcceleration;

	float fSectionWidth;
	std::list<int> listSections;
	float fLevelPosition = 0.0f;

	int nObstacleCount = 0;
	int nMaxObstacleCount = 4;

	bool bAlive = true;
	int nScore = 0;

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
