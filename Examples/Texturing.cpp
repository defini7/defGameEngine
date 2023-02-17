#include "defGameEngine.h"

#define DGE_PANANDZOOM
#include "DGE_PanAndZoom.h"

class TextureSampling : public def::GameEngine
{
public:
	TextureSampling()
	{
		SetTitle("Texture Sampling");
		ShowFPS();
	}

	enum class SampleMode : uint8_t
	{
		Point,
		BiLinear
	};

	enum class WrapMode : uint8_t
	{
		None,
		Repeat,
		Mirror,
		Clamp
	};

	def::Pixel Sample(def::Sprite* tex, def::vf2d& uv, SampleMode sample = SampleMode::Point, WrapMode wrap = WrapMode::None)
	{
		def::Pixel out = def::WHITE;

		def::vi2d vFilePos = uv * GetScreenSize();

		switch (sample)
		{
		case SampleMode::Point:
		{
			out = GetPixel(tex, vFilePos, wrap);
		}
		break;

		case SampleMode::BiLinear:
		{
			def::vf2d vRatio = uv - uv.floor();
			def::vf2d vInverted = def::vf2d(1.0f, 1.0f) - vRatio;

			def::Pixel p0 = GetPixel(tex, vFilePos + def::vi2d(0, 0), wrap);
			def::Pixel p1 = GetPixel(tex, vFilePos + def::vi2d(1, 0), wrap);
			def::Pixel p2 = GetPixel(tex, vFilePos + def::vi2d(0, 1), wrap);
			def::Pixel p3 = GetPixel(tex, vFilePos + def::vi2d(1, 1), wrap);

			out.r = (p0.r * vInverted.x + p1.r * vRatio.x) * vInverted.y + (p2.r * vInverted.x + p3.r * vRatio.x) * vRatio.y;
			out.g = (p0.g * vInverted.x + p1.g * vRatio.x) * vInverted.y + (p2.g * vInverted.x + p3.g * vRatio.x) * vRatio.y;
			out.b = (p0.b * vInverted.x + p1.b * vRatio.x) * vInverted.y + (p2.b * vInverted.x + p3.b * vRatio.x) * vRatio.y;
		}
		break;

		}

		return out;
	}

	def::Pixel GetPixel(def::Sprite* tex, const def::vi2d& pos, WrapMode wrap = WrapMode::None)
	{
		def::Pixel out = def::WHITE;

		switch (wrap)
		{
		case WrapMode::None:
		{
			if (pos.x >= 0 && pos.y >= 0 && pos.x < tex->GetWidth() && pos.y < tex->GetHeight())
				out = tex->GetPixel(pos.x, pos.y);
		}
		break;

		case WrapMode::Repeat:
		{
			def::vi2d vFilePos;

			vFilePos.x = pos.x % tex->GetWidth();
			vFilePos.y = pos.y % tex->GetHeight();

			out = tex->GetPixel(vFilePos.x, vFilePos.y);
		}
		break;

		case WrapMode::Mirror:
		{
			def::vi2d vQueue, vMirrored;

			vQueue.x = pos.x / tex->GetWidth();
			vQueue.y = pos.y / tex->GetHeight();

			vMirrored.x = pos.x % tex->GetWidth();
			vMirrored.y = pos.y % tex->GetHeight();

			if (vQueue.x % 2 == 0) vMirrored.x = tex->GetWidth() - vMirrored.x - 1;
			if (vQueue.y % 2 == 0) vMirrored.y = tex->GetHeight() - vMirrored.y - 1;

			out = tex->GetPixel(vMirrored.x, vMirrored.y);
		}
		break;

		case WrapMode::Clamp:
		{
			def::vi2d vClamped = pos.clamp({ 0, 0 }, tex->GetSize() - 1);
			out = tex->GetPixel(vClamped.x, vClamped.y);
		}
		break;

		}

		return out;
	}

private:
	SampleMode sampleMode;
	WrapMode wrapMode;

	def::PanAndZoom pz;

	std::unique_ptr<def::Sprite> sprDemo;

protected:
	bool OnUserCreate() override
	{
		pz.Initialize();

		sprDemo = std::make_unique<def::Sprite>("land.png");

		pz.SetScale(sprDemo->GetSize());

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (GetKey(def::Key::K1).bPressed)
			sampleMode = SampleMode::Point;

		if (GetKey(def::Key::K2).bPressed)
			sampleMode = SampleMode::BiLinear;

		if (GetKey(def::Key::K3).bPressed)
			wrapMode = WrapMode::None;

		if (GetKey(def::Key::K4).bPressed)
			wrapMode = WrapMode::Clamp;

		if (GetKey(def::Key::K5).bPressed)
			wrapMode = WrapMode::Mirror;

		if (GetKey(def::Key::K6).bPressed)
			wrapMode = WrapMode::Repeat;

		pz.Handle(this);

		for (int x = 0; x < GetScreenWidth(); x++)
			for (int y = 0; y < GetScreenHeight(); y++)
			{
				def::vf2d vSample = pz.ScreenToWorld({ x, y });

				def::Pixel p = Sample(sprDemo.get(), vSample, sampleMode, wrapMode);
				Draw(x, y, p);
			}

		return true;
	}
};

int main()
{
	TextureSampling demo;

	if (demo.Construct(1280, 960, 1, 1))
		demo.Run();

	return 0;
}
