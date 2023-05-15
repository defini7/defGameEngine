#define DGE_APPLICATION
#include "defGameEngine.h"

class Mode7 : public def::GameEngine
{
public:
	Mode7()
	{
		SetTitle("Mode7");
		ShowFPS();
	}

private:
	def::Sprite* sprKart = nullptr;
	def::Sprite* sprSky = nullptr;

public:
	bool OnUserCreate() override
	{
		sprKart = new def::Sprite("kart.png");
		sprSky = new def::Sprite("sky.png");

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (GetKey(def::Key::UP).bHeld) vScale += 10.0f * fDeltaTime;
		if (GetKey(def::Key::DOWN).bHeld) vScale -= 10.0f * fDeltaTime;

		if (GetKey(def::Key::LEFT).bHeld) fTheta -= fDeltaTime;
		if (GetKey(def::Key::RIGHT).bHeld) fTheta += fDeltaTime;

		def::vf2d vVelocity = { 0.0f, 0.0f };

		if (GetKey(def::Key::W).bHeld) vVelocity += def::vf2d(-sin(fTheta), cos(fTheta));
		if (GetKey(def::Key::A).bHeld) vVelocity += def::vf2d(cos(fTheta), sin(fTheta));
		if (GetKey(def::Key::S).bHeld) vVelocity += def::vf2d(sin(fTheta), -cos(fTheta));
		if (GetKey(def::Key::D).bHeld) vVelocity += def::vf2d(-cos(fTheta), -sin(fTheta));

		vCamera += vVelocity * fDeltaTime;

		def::vf2d vScreen;
		for (vScreen.x = 0; vScreen.x < ScreenWidth(); vScreen.x++)
		{
			for (vScreen.y = ScreenHeight() / 2; vScreen.y < ScreenHeight(); vScreen.y++)
			{
				def::vf2d vWindow = def::vf2d(ScreenSize()) / 2.0f + vScreen * def::vf2d(-1.0f, 1.0f);
				float fWindowZ = vScreen.y - ScreenHeight() / 2;

				float fRotatedX = vWindow.x * cos(fTheta) - vWindow.y * sin(fTheta);
				float fRotatedY = vWindow.x * sin(fTheta) + vWindow.y * cos(fTheta);

				def::vi2d vPixel = (def::vf2d(fRotatedX, fRotatedY) / fWindowZ + vCamera) * vScale;

				def::Pixel pixKart = sprKart->GetPixel(vPixel.x % sprKart->nWidth, vPixel.y % sprKart->nHeight);
				def::Pixel pixSky = sprSky->GetPixel(vPixel.x % sprSky->nWidth, vPixel.y % sprSky->nHeight);

				//float fShade = std::min(std::max(1.5f * (abs(fWindowZ) / (GetScreenHeight() / 2)), 0.0f), 1.0f);

				//pixKart.r = (float)pixKart.r * fShade;
				//pixKart.g = (float)pixKart.g * fShade;
				//pixKart.b = (float)pixKart.b * fShade;

				//pixSky.r = (float)pixSky.r * fShade;
				//pixSky.g = (float)pixSky.g * fShade;
				//pixSky.b = (float)pixSky.b * fShade;

				Draw(def::vf2d(vScreen.x, vScreen.y), pixKart);
				Draw(def::vf2d(vScreen.x, ScreenHeight() - vScreen.y - 1), pixSky);
			}
		}

		for (int x = 0; x < ScreenWidth(); x++) Draw(x, ScreenHeight() / 2, def::CYAN);

		return true;
	}

private:
	def::vf2d vCamera = { 0.0f, 0.0f };
	def::vf2d vScale = { 100.0f, 100.0f };
	float fTheta = 0.0f;

};

int main()
{
	Mode7 app;

	if (app.Construct(256, 240, 4, 4))
		app.Run();

	return 0;
}
