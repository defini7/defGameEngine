#include "defGameEngine.h"

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Sample");
		ShowFPS();
	}

private:
	def::vf2d vScale;
	def::vf2d vOffset;

	def::vf2d vStartPan;

protected:
	bool OnUserCreate() override
	{
		vScale = { 1.0f, 1.0f };
		vOffset = { 0.0f, 0.0f };

		return true;
	}

	def::vi2d WorldToScreen(const def::vf2d& vWorld)
	{
		return ((def::vf2d)vWorld - vOffset) * vScale;
	}

	def::vf2d ScreenToWorld(const def::vi2d& vScreen)
	{
		return (def::vf2d)vScreen / vScale + vOffset;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		def::vf2d vMouse = GetMouse();

		if (GetMouse(2).bPressed)
			vStartPan = vMouse;

		if (GetMouse(2).bHeld)
		{
			vOffset -= (vMouse - vStartPan) / vScale;
			vStartPan = vMouse;
		}

		def::vf2d vBeforeZoom = ScreenToWorld(vMouse);

		if (GetKey(def::Key::EQUAL).bHeld)
			vScale *= 1.001f;

		if (GetKey(def::Key::MINUS).bHeld)
			vScale *= 0.999f;

		vScale.x = std::max(vScale.x, 1.0f);
		vScale.y = std::max(vScale.y, 1.0f);

		def::vf2d vAfterZoom = ScreenToWorld(vMouse);
		vOffset += vBeforeZoom - vAfterZoom;

		Clear(def::BLACK);

		def::vi2d vLineLeft = WorldToScreen({ 0.0f, 0.5f * (float)GetScreenHeight() });
		def::vi2d vLineRight = WorldToScreen({ (float)GetScreenWidth(), 0.5f * (float)GetScreenHeight() });

		def::vi2d vLineTop = WorldToScreen({ 0.5f * (float)GetScreenWidth(), 0.0f });
		def::vi2d vLineBottom = WorldToScreen({ 0.5f * (float)GetScreenWidth(), (float)GetScreenHeight() });

		DrawLine(vLineLeft, vLineRight);
		DrawLine(vLineTop, vLineBottom);
		
		def::vf2d vWorldTopLeft = ScreenToWorld({ 0, 0 });
		def::vf2d vWorldTopRight = ScreenToWorld(GetScreenSize());

		def::vf2d vWorldMouse = vAfterZoom;

		def::vf2d vWorldPerScreenSizePixel = (vWorldTopRight - vWorldTopLeft) / GetScreenSize();

		auto func = [](float x)
		{
			return sinf(x);
		};

		def::vi2d p, op;
		op = WorldToScreen({ vWorldTopLeft.x - vWorldPerScreenSizePixel.x, -func(vWorldTopLeft.x - vWorldPerScreenSizePixel.x - 0.5f * (float)GetScreenWidth()) + 0.5f * (float)GetScreenHeight() });
		
		for (float x = vWorldTopLeft.x; x < vWorldTopRight.x; x += vWorldPerScreenSizePixel.x)
		{
			float y = -func(x - 0.5f * (float)GetScreenWidth()) + 0.5f * (float)GetScreenHeight();
			p = WorldToScreen({ x, y });
			DrawLine(op, p, def::GREEN);
			op = p;
		}

		DrawString(5, 8, "Offset: " + std::string(vOffset));
		DrawString(5, 18, "Scale: " + std::string(vScale));

		return true;
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
