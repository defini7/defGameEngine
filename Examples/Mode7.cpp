#define DGE_APPLICATION
#include "../defGameEngine.hpp"

class Mode7 : public def::GameEngine
{
public:
	Mode7()
	{
		SetTitle("Mode7");
	}

private:
	def::Sprite* kart = nullptr;
	def::Sprite* sky = nullptr;

public:
	bool OnUserCreate() override
	{
		kart = new def::Sprite("assets/kart.png");
		sky = new def::Sprite("assets/sky.png");

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetKey(def::Key::UP).held) scale += 10.0f * deltaTime;
		if (GetKey(def::Key::DOWN).held) scale -= 10.0f * deltaTime;

		if (GetKey(def::Key::LEFT).held) theta -= deltaTime;
		if (GetKey(def::Key::RIGHT).held) theta += deltaTime;

		def::vf2d velocity = { 0.0f, 0.0f };

		if (GetKey(def::Key::W).held) velocity += def::vf2d(-sin(theta), cos(theta));
		if (GetKey(def::Key::A).held) velocity += def::vf2d(cos(theta), sin(theta));
		if (GetKey(def::Key::S).held) velocity += def::vf2d(sin(theta), -cos(theta));
		if (GetKey(def::Key::D).held) velocity += def::vf2d(-cos(theta), -sin(theta));

		camera += velocity * deltaTime;

		def::vf2d screen;
		for (screen.x = 0; screen.x < ScreenWidth(); screen.x++)
		{
			for (screen.y = ScreenHeight() / 2; screen.y < ScreenHeight(); screen.y++)
			{
				def::vf2d window = def::vf2d(GetScreenSize()) / 2.0f + screen * def::vf2d(-1.0f, 1.0f);
				float windowZ = screen.y - ScreenHeight() / 2;

				float rotatedX = window.x * cos(theta) - window.y * sin(theta);
				float rotatedY = window.x * sin(theta) + window.y * cos(theta);

				def::vi2d vPixel = (def::vf2d(rotatedX, rotatedY) / windowZ + camera) * scale;

				def::Pixel kartCol = kart->GetPixel(vPixel.x % kart->size.x, vPixel.y % kart->size.y);
				def::Pixel skyCol = sky->GetPixel(vPixel.x % sky->size.x, vPixel.y % sky->size.y);

				Draw(screen, kartCol);
				Draw(def::vf2d(screen.x, ScreenHeight() - screen.y - 1), skyCol);
			}
		}

		for (int x = 0; x < ScreenWidth(); x++) Draw(x, ScreenHeight() / 2, def::CYAN);

		return true;
	}

private:
	def::vf2d camera = { 0.0f, 0.0f };
	def::vf2d scale = { 100.0f, 100.0f };
	float theta = 0.0f;

};

int main()
{
	Mode7 app;

	app.Construct(256, 240, 4, 4);
	app.Run();

	return 0;
}
