#include "defGameEngine.h"

#define DGE_PANANDZOOM
#include "DGE_PanAndZoom.h"

const def::Pixel g_pBackground = def::DARK_CYAN;

class CustomTexture
{
public:
	CustomTexture(const std::string& fileName, const def::vf2d& borderStart, const def::vf2d& borderSize)
	{
		m_Spr = std::make_unique<def::Sprite>(fileName);

		vBorderStart = borderStart;
		vBorderSize = borderSize;
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

	def::Pixel Sample(def::vf2d& uv, SampleMode sample = SampleMode::Point, WrapMode wrap = WrapMode::None)
	{
		def::Pixel out = g_pBackground;

		def::vf2d vFilePos = vBorderStart + uv * vBorderSize;

		switch (sample)
		{
		case SampleMode::Point:
		{
			out = GetPixel(vFilePos, wrap);
		}
		break;

		case SampleMode::BiLinear:
		{
			def::vf2d vOffset = vFilePos - vFilePos.floor();

			def::Pixel pTL = GetPixel(vFilePos + def::vi2d(0, 0), wrap);
			def::Pixel pTR = GetPixel(vFilePos + def::vi2d(1, 0), wrap);
			def::Pixel pBL = GetPixel(vFilePos + def::vi2d(0, 1), wrap);
			def::Pixel pBR = GetPixel(vFilePos + def::vi2d(1, 1), wrap);

			def::Pixel pTop = pTR * vOffset.x + pTL * (1.0f - vOffset.x);
			def::Pixel pBottom = pBR * vOffset.x + pBL * (1.0f - vOffset.x);

			out = pBottom * vOffset.y + pTop * (1.0f - vOffset.y);
		}
		break;

		}

		return out;
	}

	def::Pixel GetPixel(const def::vi2d& pos, WrapMode wrap = WrapMode::None)
	{
		def::Pixel out = g_pBackground;

		switch (wrap)
		{
		case WrapMode::None:
		{
			if (pos.x >= 0 && pos.y >= 0 && pos.x < m_Spr->GetWidth() && pos.y < m_Spr->GetHeight())
				out = m_Spr->GetPixel(pos.x, pos.y);
		}
		break;

		case WrapMode::Repeat:
		{
			def::vi2d vFilePos;

			vFilePos.x = pos.x % m_Spr->GetWidth();
			vFilePos.y = pos.y % m_Spr->GetHeight();

			out = m_Spr->GetPixel(vFilePos.x, vFilePos.y);
		}
		break;

		case WrapMode::Mirror:
		{
			def::vi2d vQueue, vMirrored;

			vQueue.x = pos.x / m_Spr->GetWidth();
			vQueue.y = pos.y / m_Spr->GetHeight();

			vMirrored.x = pos.x % m_Spr->GetWidth();
			vMirrored.y = pos.y % m_Spr->GetHeight();

			if (vQueue.x % 2 == 0) vMirrored.x = m_Spr->GetWidth() - vMirrored.x - 1;
			if (vQueue.y % 2 == 0) vMirrored.y = m_Spr->GetHeight() - vMirrored.y - 1;

			out = m_Spr->GetPixel(vMirrored.x, vMirrored.y);
		}
		break;

		case WrapMode::Clamp:
		{
			def::vi2d vClamped = pos.clamp({ 0, 0 }, m_Spr->GetSize() - 1);
			out = m_Spr->GetPixel(vClamped.x, vClamped.y);
		}
		break;

		}

		return out;
	}

	def::vi2d GetSize() const
	{
		return m_Spr->GetSize();
	}

public:
	def::vf2d vBorderStart;
	def::vf2d vBorderSize;

private:
	std::unique_ptr<def::Sprite> m_Spr;

};

class TextureSampling : public def::GameEngine
{
public:
	TextureSampling()
	{
		SetTitle("Texture Sampling");
		ShowFPS();
	}





private:
	def::PanAndZoom pz;

	std::unique_ptr<CustomTexture> tex1;
	std::unique_ptr<CustomTexture> tex2;

	CustomTexture::SampleMode sampleMode;
	CustomTexture::WrapMode wrapMode;

protected:
	bool OnUserCreate() override
	{
		pz.Initialize(this);

		tex1 = std::make_unique<CustomTexture>("land.jpg", def::vf2d(0, 0), def::vf2d(1280, 960));
		tex2 = std::make_unique<CustomTexture>("perlin0.jpg", def::vf2d(0, 0), def::vf2d(1280, 960));

		pz.SetScale(def::vf2d(1280, 960));

		return true;
	}

	void DrawCustomTexture(std::unique_ptr<CustomTexture>& texture)
	{
		for (int x = 0; x < GetScreenWidth(); x++)
			for (int y = 0; y < GetScreenHeight(); y++)
			{
				def::vf2d vSample = pz.ScreenToWorld({ x, y });

				def::Pixel p = texture.get()->Sample(vSample, sampleMode, wrapMode);
				Draw(x, y, p);
			}
	}

	template <typename T>
	T Mix(T x, T y, float a)
	{
		return x * (1.0f - a) + y * a;
	}

	void DrawMixedCustomTexture(std::unique_ptr<CustomTexture>& texture1, std::unique_ptr<CustomTexture>& texture2, float force = 0.5f)
	{
		for (int x = 0; x < GetScreenWidth(); x++)
			for (int y = 0; y < GetScreenHeight(); y++)
			{
				def::vf2d vSample = pz.ScreenToWorld({ x, y });

				def::Pixel p1 = texture1.get()->Sample(vSample, sampleMode, wrapMode);
				def::Pixel p2 = texture2.get()->Sample(vSample, sampleMode, wrapMode);

				def::Pixel mixed;
				mixed.r = Mix(p1.r, p2.r, force);
				mixed.g = Mix(p1.g, p2.g, force);
				mixed.b = Mix(p1.b, p2.b, force);

				Draw(x, y, mixed);
			}
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (GetKey(def::Key::K1).bPressed)
			sampleMode = CustomTexture::SampleMode::Point;

		if (GetKey(def::Key::K2).bPressed)
			sampleMode = CustomTexture::SampleMode::BiLinear;

		if (GetKey(def::Key::K3).bPressed)
			wrapMode = CustomTexture::WrapMode::None;

		if (GetKey(def::Key::K4).bPressed)
			wrapMode = CustomTexture::WrapMode::Clamp;

		if (GetKey(def::Key::K5).bPressed)
			wrapMode = CustomTexture::WrapMode::Mirror;

		if (GetKey(def::Key::K6).bPressed)
			wrapMode = CustomTexture::WrapMode::Repeat;

		pz.Handle();

		//DrawCustomTexture(tex1);
		DrawCustomTexture(tex2);

		//DrawMixedCustomTexture(tex1, tex2);

		pz.DrawRectangle(0.0f, 0.0f, 1.0f, 1.0f, def::WHITE);
		
		DrawString(8, 8, "Sample mode: Point - 1, BiLinear - 2");
		DrawString(8, 16, "Wrap mode: None - 3, Clamp - 4, Mirror - 5, Repeat - 6");

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
