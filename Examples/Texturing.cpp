#include "defGameEngine.h"

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
		Point
	};

	enum class WrapMode : uint8_t
	{
		None,
		Repeat,
		Mirror,
		Clamp
	};
	
	def::Texture* texDemo;

	def::Pixel Sample(def::Texture* tex, def::vf2d& uv, SampleMode sample = SampleMode::Point, WrapMode wrap = WrapMode::None)
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
		
		}

		return out;
	}

	def::Pixel GetPixel(def::Texture* tex, const def::vi2d& pos, WrapMode wrap = WrapMode::None)
	{
		def::Pixel out = def::WHITE;

		switch (wrap)
		{
		case WrapMode::None:
		{
			if (pos.x >= 0 && pos.y >= 0 && pos.x < tex->Spr()->GetWidth() && pos.y < tex->Spr()->GetHeight())
				out = tex->Spr()->GetPixel(pos.x, pos.y);
		}
		break;

		case WrapMode::Repeat:
		{
			def::vi2d vFilePos;

			vFilePos.x = pos.x % tex->Spr()->GetWidth();
			vFilePos.y = pos.y % tex->Spr()->GetHeight();

			out = tex->Spr()->GetPixel(vFilePos.x, vFilePos.y);
		}
		break;

		case WrapMode::Mirror:
		{
			def::vi2d vQueue, vMirrored;

			vQueue.x = pos.x / tex->Spr()->GetWidth();
			vQueue.y = pos.y / tex->Spr()->GetHeight();

			vMirrored.x = pos.x % tex->Spr()->GetWidth();
			vMirrored.y = pos.y % tex->Spr()->GetHeight();

			if (vQueue.x % 2 == 0) vMirrored.x = tex->Spr()->GetWidth() - vMirrored.x - 1;
			if (vQueue.y % 2 == 0) vMirrored.y = tex->Spr()->GetHeight() - vMirrored.y - 1;

			out = tex->Spr()->GetPixel(vMirrored.x, vMirrored.y);
		}
		break;

		case WrapMode::Clamp:
		{
			def::vi2d vClamped = pos.clamp({ 0, 0 }, tex->Spr()->GetSize() - 1);
			out = tex->Spr()->GetPixel(vClamped.x, vClamped.y);
		}
		break;

		}

		return out;
	}

protected:
	bool OnUserCreate() override
	{
		texDemo = new def::Texture("udxs_building1.png");
		
		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		for (int x = 0; x < GetScreenWidth(); x++)
			for (int y = 0; y < GetScreenHeight(); y++)
			{
				def::vf2d vSample = {
					(float)x / (float)GetScreenWidth(),
					(float)y / (float)GetScreenHeight()
				};

				def::Pixel p = Sample(texDemo, vSample, SampleMode::Point, WrapMode::None);
				Draw(x, y, p);
			}

		return true;
	}
};

int main()
{
	TextureSampling demo;

	def::rcode err = demo.Construct(1280, 960, 1, 1);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
