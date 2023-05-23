#define DGE_APPLICATION
#include "defGameEngine.h"

struct mat2x2
{
	float* operator[](size_t i) { return m[i]; }

	float m[2][2]{ 0 };
};

void Mat_MultiplyMat(mat2x2& matA, mat2x2& matB, mat2x2& matResult)
{
	for (size_t i = 0; i < 2; i++)
		for (size_t j = 0; j < 2; j++)
		{
			for (size_t k = 0; k < 2; k++)
				matResult[i][j] += matA[i][k] * matB[k][j];
		}
}

void Mat_MultiplyVec(mat2x2& matA, def::vf2d& vecB, def::vf2d& vResult)
{
	vResult.x = matA[0][0] * vecB.x + matA[0][1] * vecB.y;
	vResult.y = matA[1][0] * vecB.x + matA[1][1] * vecB.y;
}

void Mat_MakeRotation(mat2x2& matA, const float fAngle)
{
	float s = sin(fAngle), c = cos(fAngle);
	matA[0][0] = c;
	matA[0][1] = s;
	matA[1][0] = -s;
	matA[1][1] = c;
}

void Mat_MakeScale(mat2x2& matA, const def::vf2d& vScale)
{
	matA[0][0] = vScale.x;
	matA[0][1] = 0.0f;
	matA[1][0] = 0.0f;
	matA[1][1] = vScale.y;
}

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Sample");
	}

protected:
	bool OnUserCreate() override
	{
		sprDemo = new def::Sprite("road.jpg");
		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		mat2x2 matRotated, matScaled;

		Mat_MakeRotation(matRotated, fTheta);
		Mat_MakeScale(matScaled, { 0.5f, 0.5f });

		Clear(def::BLACK);

		def::vf2d vPos;
		for (vPos.y = 0; vPos.y < sprDemo->nHeight; vPos.y++)
			for (vPos.x = 0; vPos.x < sprDemo->nWidth; vPos.x++)
			{
				def::vf2d vScaled;
				Mat_MultiplyVec(matScaled, vPos, vScaled);

				def::vf2d vRotated;
				Mat_MultiplyVec(matRotated, vScaled, vRotated);

				def::vf2d vTranslated;
				vTranslated = vRotated + def::vf2d(ScreenWidth() * 0.5f, ScreenHeight() * 0.5f);

				Draw(vTranslated, sprDemo->GetPixel(vPos.x, vPos.y));
			}

		fTheta += fDeltaTime;
		return true;
	}

private:
	def::Sprite* sprDemo = nullptr;
	float fTheta = 0.0f;
};

int main()
{
	Sample demo;

	if (demo.Construct(1280, 720, 1, 1))
		demo.Run();

	return 0;
}
