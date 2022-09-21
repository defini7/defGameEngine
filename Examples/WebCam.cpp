#include "defGameEngine.h"
#include "escapi.h"

class WebCam : public def::GameEngine
{
public:
	WebCam()
	{
		SetTitle("Web Cam");
	}

private:
	int nCameras = 0;
	SimpleCapParams scpCapture;

protected:
	void ApplyThreshold(def::Pixel& p)
	{
		if (p.g > 122)
		{
			p.r = 255;
			p.g = 255;
			p.b = 255;
		}
		else
		{
			p.r = 0;
			p.g = 0;
			p.b = 0;
		}
	}

	void ApplySepia(def::Pixel& p)
	{
		int sr = int(0.393f * p.r + 0.769f * p.g + 0.189f * p.b);
		int sg = int(0.349f * p.r + 0.686f * p.g + 0.168f * p.b);
		int sb = int(0.272f * p.r + 0.534f * p.g + 0.131f * p.b);

		p.r = (sr > 255) ? 255 : sr;
		p.g = (sg > 255) ? 255 : sg;
		p.b = (sb > 255) ? 255 : sb;
	}

	void ApplyGreyscale(def::Pixel& p)
	{
		int n = (p.r + p.g + p.b) / 3;

		p.r = n;
		p.g = n;
		p.b = n;
	}

	bool OnUserCreate() override
	{
		nCameras = setupESCAPI();

		if (nCameras == 0)
			return false;

		scpCapture.mWidth = GetScreenWidth();
		scpCapture.mHeight = GetScreenHeight();
		
		scpCapture.mTargetBuf = new int[GetScreenWidth() * GetScreenHeight()];

		if (!initCapture(0, &scpCapture))
			return false;
		
		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		doCapture(0);

		for (int x = 0; x < scpCapture.mWidth; x++)
			for (int y = 0; y < scpCapture.mHeight; y++)
			{
				union RGBAint
				{
					int rgba;
					uint8_t c[4];
				};

				RGBAint col;
				col.rgba = scpCapture.mTargetBuf[y * scpCapture.mWidth + x];

				def::Pixel p = def::Pixel(col.c[2], col.c[1], col.c[0], 255);

				if (GetKey(def::Key::G).bHeld)
					ApplyGreyscale(p);

				if (GetKey(def::Key::S).bHeld)
					ApplySepia(p);

				if (GetKey(def::Key::T).bHeld)
					ApplyThreshold(p);

				Draw(x, y, p);
			}

		return true;
	}

};

int main()
{
	WebCam demo;
	def::rcode err = demo.Construct(256, 240, 4, 4);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
