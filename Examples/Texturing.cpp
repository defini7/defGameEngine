/*
* BSD 3-Clause License
Copyright (c) 2023, Alex
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define DGE_APPLICATION
#include "../defGameEngine.h"

#define DGE_PANANDZOOM
#include "../Extensions/DGE_PanAndZoom.h"

const def::Pixel g_BackgroundCol = def::DARK_CYAN;

class CustomTexture
{
public:
	CustomTexture(const std::string& fileName, const def::vf2d& borderStart, const def::vf2d& borderSize)
	{
		m_Spr = std::make_unique<def::Sprite>(fileName);

		this->borderStart = borderStart;
		this->borderSize = borderSize;
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
		def::Pixel out = g_BackgroundCol;
		def::vf2d filePos = borderStart + uv * borderSize;

		switch (sample)
		{
		case SampleMode::Point:
			out = GetPixel(filePos.floor(), wrap);
		break;

		case SampleMode::BiLinear:
		{
			def::vf2d offset = filePos - filePos.floor();

			def::Pixel tl = GetPixel(filePos + def::vf2d(0, 0), wrap);
			def::Pixel tr = GetPixel(filePos + def::vf2d(1, 0), wrap);
			def::Pixel bl = GetPixel(filePos + def::vf2d(0, 1), wrap);
			def::Pixel br = GetPixel(filePos + def::vf2d(1, 1), wrap);

			def::Pixel topCol = tr * offset.x + tl * (1.0f - offset.x);
			def::Pixel bottomCol = br * offset.x + bl * (1.0f - offset.x);

			out = bottomCol * offset.y + topCol * (1.0f - offset.y);
		}
		break;

		}

		return out;
	}

	def::Pixel GetPixel(const def::vi2d& pos, WrapMode wrap = WrapMode::None)
	{
		def::Pixel out = g_BackgroundCol;

		switch (wrap)
		{
		case WrapMode::None:
		{
			if (pos > def::vi2d(0, 0) && pos < def::vi2d(m_Spr->width, m_Spr->height))
				out = m_Spr->GetPixel(pos.x, pos.y);
		}
		break;

		case WrapMode::Repeat:
		{
			def::vi2d vFilePos;

			vFilePos.x = pos.x % m_Spr->width;
			vFilePos.y = pos.y % m_Spr->height;

			out = m_Spr->GetPixel(vFilePos.x, vFilePos.y);
		}
		break;

		case WrapMode::Mirror:
		{
			def::vi2d queue, mirrored;

			queue.x = pos.x / m_Spr->width;
			queue.y = pos.y / m_Spr->height;

			mirrored.x = pos.x % m_Spr->width;
			mirrored.y = pos.y % m_Spr->height;

			if (queue.x % 2 == 0) mirrored.x = m_Spr->width - mirrored.x - 1;
			if (queue.y % 2 == 0) mirrored.y = m_Spr->height - mirrored.y - 1;

			out = m_Spr->GetPixel(mirrored.x, mirrored.y);
		}
		break;

		case WrapMode::Clamp:
		{
			def::vi2d clamped = pos.clamp({ 0, 0 }, { m_Spr->width - 1, m_Spr->height - 1 });
			out = m_Spr->GetPixel(clamped.x, clamped.y);
		}
		break;

		}

		return out;
	}

	def::vi2d GetSize() const
	{
		return { m_Spr->width, m_Spr->height };
	}

public:
	def::vf2d borderStart;
	def::vf2d borderSize;

private:
	std::unique_ptr<def::Sprite> m_Spr;

};

class TextureSampling : public def::GameEngine
{
public:
	TextureSampling()
	{
		SetTitle("Texture Sampling");
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
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
			{
				def::Pixel p = texture.get()->Sample(pz.ScreenToWorld({ x, y }), sampleMode, wrapMode);
				Draw(x, y, p);
			}
	}

	void DrawMixedCustomTexture(std::unique_ptr<CustomTexture>& texture1, std::unique_ptr<CustomTexture>& texture2, float force = 0.5f)
	{
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
			{
				def::vf2d sample = pz.ScreenToWorld({ x, y });

				def::Pixel p1 = texture1.get()->Sample(sample, sampleMode, wrapMode);
				def::Pixel p2 = texture2.get()->Sample(sample, sampleMode, wrapMode);

				Draw(x, y, p1.mix(p2, 0.5f));
			}
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (GetKey(def::Key::K1).pressed)
			sampleMode = CustomTexture::SampleMode::Point;

		if (GetKey(def::Key::K2).pressed)
			sampleMode = CustomTexture::SampleMode::BiLinear;

		if (GetKey(def::Key::K3).pressed)
			wrapMode = CustomTexture::WrapMode::None;

		if (GetKey(def::Key::K4).pressed)
			wrapMode = CustomTexture::WrapMode::Clamp;

		if (GetKey(def::Key::K5).pressed)
			wrapMode = CustomTexture::WrapMode::Mirror;

		if (GetKey(def::Key::K6).pressed)
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

	demo.Construct(1280, 960, 1, 1);
	demo.Run();

	return 0;
}
