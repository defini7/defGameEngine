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
#include "escapi.h"

#include <algorithm>
#include <array>

constexpr int FRAME_WIDTH = 320;
constexpr int FRAME_HEIGHT = 240;

constexpr float CONVO_RIDGE_KERNEL[9] =
{
	0.0f, -1.0f, 0.0f,
	-1.0f, 4.0f, -1.0f,
	0.0f, -1.0f, 0.0f
};

constexpr float CONVO_EDGE_KERNEL[9] =
{
	-1.0f, -1.0f, -1.0f,
	-1.0f,  8.0f, -1.0f,
	-1.0f, -1.0f, -1.0f
};

constexpr float CONVO_SHARPEN_KERNEL[9] =
{
	0.0f, -1.0f, 0.0f,
	-1.0f, 5.0f, -1.0f,
	0.0f, -1.0f, 0.0f
};

constexpr float CONVO_BLUR_KERNEL[9] =
{
	1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
	1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
	1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f
};

constexpr float SOBEL_KERNEL_X[9] =
{
	-1.0f, 0.0f, 1.0f,
	-2.0f, 0.0f, 2.0f,
	-1.0f, 0.0f, 1.0f
};

constexpr float SOBEL_KERNEL_Y[9] =
{
	-1.0f, -2.0f, -1.0f,
	0.0f, 0.0f, 0.0f,
	1.0f, 2.0f, 1.0f
};

constexpr float DITHERING_FLOYDSTEINBERG_KERNEL[4] =
{
	7.0f / 16.0f,
	3.0f / 16.0f,
	5.0f / 16.0f,
	1.0f / 16.0f
};

struct Pixelf
{
	Pixelf(float r = 0.0f, float g = 0.0f, float b = 0.0f) : r(r), g(g), b(b) {}
	float r, g, b;
};

struct Frame
{
	Pixelf* pixels = nullptr;

	Frame()
	{
		pixels = new Pixelf[FRAME_WIDTH * FRAME_HEIGHT];
	}

	~Frame()
	{
		delete[] pixels;
	}

	Pixelf get(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < FRAME_WIDTH && y < FRAME_HEIGHT)
			return pixels[y * FRAME_WIDTH + x];

		return { 0.0f, 0.0f, 0.0f };
	}

	void set(int x, int y, const Pixelf& p)
	{
		if (x >= 0 && y >= 0 && x < FRAME_WIDTH && y < FRAME_HEIGHT)
			pixels[y * FRAME_WIDTH + x] = p;
	}

	Frame& operator=(const Frame& f)
	{
		memcpy(pixels, f.pixels, FRAME_WIDTH * FRAME_HEIGHT * sizeof(Pixelf));
		return *this;
	}

};

void DitheringQuantise_NBit(const Pixelf& in, Pixelf& out, const int bitsPerChannel)
{
	float levels = (1 << bitsPerChannel) - 1;
	out.r = std::clamp(round(in.r * levels) / levels, 0.0f, 1.0f);
	out.g = std::clamp(round(in.g * levels) / levels, 0.0f, 1.0f);
	out.b = std::clamp(round(in.b * levels) / levels, 0.0f, 1.0f);
}

void DitheringQuantise_CustomPalette(const Pixelf& in, Pixelf& out, const std::vector<Pixelf>& shades)
{
	float closest = INFINITY;

	for (const auto& s : shades)
	{
		float dist = sqrt(pow(s.r - in.r, 2) + pow(s.g - in.g, 2) + pow(s.b - in.b, 2));

		if (dist < closest)
		{
			closest = dist;
			out = s;
		}
	}
}

class WebCam : public def::GameEngine
{
public:
	WebCam()
	{
		SetTitle("Web Cam");
	}

private:
	SimpleCapParams capture;

	Frame input, output, prevInput, motionDiffMap;

	union RGBint
	{
		int rgb;
		uint8_t c[4];
	};

	enum class Filter
	{
		Threshold,
		Motion,
		Convolution,
		LowPass,
		Adaptive,
		Sobel,
		Median,
		Dithering_FloydSteinberg,
		Ripple
	};

	enum class DitheringQuantiseMode
	{
		NBit,
		CustomPalette
	};

	Filter filter = Filter::Threshold;
	DitheringQuantiseMode quantMode = DitheringQuantiseMode::NBit;

	float threshold = 0.5f;
	float lowPass = 0.1f;
	float adaptive = 0.5f;

	const float* convoKernel = CONVO_RIDGE_KERNEL;

	int bitsPerChannel = 3;

	float accTime = 0.0f;
	float rippleCenterX = 0.5f;
	float rippleCenterY = 0.5f;

	const std::vector<Pixelf> DITH_PALETTE = {
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.706f, 0.0f, 0.62f }
	};

protected:
	void DrawFrame(Frame& f, int x, int y)
	{
		for (int i = 0; i < FRAME_WIDTH; i++)
			for (int j = 0; j < FRAME_HEIGHT; j++)
			{
				float r = std::min(std::max(0.0f, f.pixels[j * FRAME_WIDTH + i].r), 1.0f);
				float g = std::min(std::max(0.0f, f.pixels[j * FRAME_WIDTH + i].g), 1.0f);
				float b = std::min(std::max(0.0f, f.pixels[j * FRAME_WIDTH + i].b), 1.0f);
				Draw(x + i, y + j, def::PixelF(r, g, b, 1.0f));
			}
	}

	bool OnUserCreate() override
	{
		if (setupESCAPI() == 0) return false;
		capture.mWidth = FRAME_WIDTH;
		capture.mHeight = FRAME_HEIGHT;
		capture.mTargetBuf = new int[FRAME_WIDTH * FRAME_HEIGHT];
		return initCapture(0, &capture);
	}

	bool OnUserUpdate(float dt) override
	{
		prevInput = input;
		doCapture(0); while (isCaptureDone(0) == 0);
		for (int y = 0; y < capture.mHeight; y++)
			for (int x = 0; x < capture.mWidth; x++)
			{
				int i = y * capture.mWidth + x;
				RGBint col; col.rgb = capture.mTargetBuf[i];
				input.pixels[i].r = (float)col.c[2] / 255.0f;
				input.pixels[i].g = (float)col.c[1] / 255.0f;
				input.pixels[i].b = (float)col.c[0] / 255.0f;
			}

		if (GetKey(def::Key::K1).pressed) filter = Filter::Threshold;
		if (GetKey(def::Key::K2).pressed) filter = Filter::Motion;
		if (GetKey(def::Key::K3).pressed) filter = Filter::Convolution;
		if (GetKey(def::Key::K4).pressed) filter = Filter::LowPass;
		if (GetKey(def::Key::K5).pressed) filter = Filter::Sobel;
		if (GetKey(def::Key::K6).pressed) filter = Filter::Median;
		if (GetKey(def::Key::K7).pressed) filter = Filter::Adaptive;
		if (GetKey(def::Key::K8).pressed) filter = Filter::Dithering_FloydSteinberg;
		if (GetKey(def::Key::K9).pressed) filter = Filter::Ripple;

		switch (filter)
		{
		case Filter::Threshold:
		{
			if (GetKey(def::Key::Z).held) threshold -= 0.1f * dt;
			if (GetKey(def::Key::X).held) threshold += 0.1f * dt;

			threshold = std::min(1.0f, std::max(threshold, 0.0f));

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					Pixelf out, in = input.get(x, y);

					float col = ((in.r + in.g + in.b) / 3.0f) >= threshold ? 1.0f : 0.0f;
					out.r = col; out.g = col; out.b = col;

					output.set(x, y, out);
				}
		}
		break;

		case Filter::Motion:
		{
			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					Pixelf in = input.get(x, y);
					Pixelf prev = prevInput.get(x, y);
					
					Pixelf out;
					out.r = fabs(in.r - prev.r);
					out.g = fabs(in.g - prev.g);
					out.b = fabs(in.b - prev.b);

					output.set(x, y, out);
				}
		}
		break;

		case Filter::Convolution:
		{
			if (GetKey(def::Key::Z).held) convoKernel = CONVO_RIDGE_KERNEL;
			if (GetKey(def::Key::X).held) convoKernel = CONVO_EDGE_KERNEL;
			if (GetKey(def::Key::C).held) convoKernel = CONVO_SHARPEN_KERNEL;
			if (GetKey(def::Key::V).held) convoKernel = CONVO_BLUR_KERNEL;

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

					for (int i = 0; i < 3; i++)
						for (int j = 0; j < 3; j++)
						{
							Pixelf in = input.get(x + i - 1, y + j - 1);
							sumR += in.r * convoKernel[j * 3 + i];
							sumG += in.g * convoKernel[j * 3 + i];
							sumB += in.b * convoKernel[j * 3 + i];
						}

					output.set(x, y, { sumR, sumG, sumB });
				}
		}
		break;

		case Filter::LowPass:
		{
			if (GetKey(def::Key::Z).held) lowPass -= 0.1f * dt;
			if (GetKey(def::Key::X).held) lowPass += 0.1f * dt;

			lowPass = std::min(1.0f, std::max(lowPass, 0.0f));

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					Pixelf in1 = input.get(x, y);
					Pixelf in2 = output.get(x, y);

					Pixelf out;
					out.r = (in1.r - in2.r) * lowPass + in2.r;
					out.g = (in1.g - in2.g) * lowPass + in2.g;
					out.b = (in1.b - in2.b) * lowPass + in2.b;

					output.set(x, y, out);
				}
		}
		break;

		case Filter::Adaptive:
		{
			if (GetKey(def::Key::Z).held) adaptive -= 0.1f * dt;
			if (GetKey(def::Key::X).held) adaptive += 0.1f * dt;

			adaptive = std::min(1.0f, std::max(adaptive, 0.5f));

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

					for (int i = -2; i <= 2; i++)
						for (int j = -2; j <= 2; j++)
						{
							Pixelf in = input.get(x + i, y + j);
							sumR += in.r; sumG += in.g; sumB += in.b;
						}
					sumR /= 25.0f; sumG /= 25.0f; sumB /= 25.0f;

					Pixelf out, in = input.get(x, y);
					out.r = (in.r > sumR * adaptive) ? 1.0f : 0.0f;
					out.g = (in.g > sumG * adaptive) ? 1.0f : 0.0f;
					out.b = (in.b > sumB * adaptive) ? 1.0f : 0.0f;

					output.set(x, y, out);
				}
		}
		break;

		case Filter::Sobel:
		{
			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float sumXr = 0.0f, sumXg = 0.0f, sumXb = 0.0f;
					float sumYr = 0.0f, sumYg = 0.0f, sumYb = 0.0f;

					for (int i = 0; i < 3; i++)
						for (int j = 0; j < 3; j++)
						{
							Pixelf in = input.get(x + i - 1, y + j - 1);
							size_t k = j * 3 + i;

							sumXr += in.r * SOBEL_KERNEL_X[k];
							sumXg += in.g * SOBEL_KERNEL_X[k];
							sumXb += in.b * SOBEL_KERNEL_X[k];

							sumYr += in.r * SOBEL_KERNEL_Y[k];
							sumYg += in.g * SOBEL_KERNEL_Y[k];
							sumYb += in.b * SOBEL_KERNEL_Y[k];
						}

					Pixelf out;
					out.r = fabs((sumXr + sumYr) * 0.5f);
					out.g = fabs((sumXg + sumYg) * 0.5f);
					out.b = fabs((sumXb + sumYb) * 0.5f);

					output.set(x, y, out);
				}
		}
		break;

		case Filter::Median:
		{
			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					std::array<Pixelf, 25> col;

					for (int i = 0; i < 5; i++)
						for (int j = 0; j < 5; j++)
							col[j * 5 + i] = input.get(x + i - 2, y + j - 2);

					std::sort(col.begin(), col.end(), [](const Pixelf& c1, const Pixelf& c2) {
						return c2.r < c1.r && c2.g < c1.g && c2.b < c1.b;
					});

					output.set(x, y, col[12]);
				}

		}
		break;

		case Filter::Dithering_FloydSteinberg:
		{
			if (GetKey(def::Key::Z).pressed) bitsPerChannel--;
			if (GetKey(def::Key::X).pressed) bitsPerChannel++;
			if (GetKey(def::Key::C).pressed) quantMode = DitheringQuantiseMode::NBit;
			if (GetKey(def::Key::V).pressed) quantMode = DitheringQuantiseMode::CustomPalette;

			bitsPerChannel = std::min(std::max(bitsPerChannel, 1), 31);

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					Pixelf oldPix = input.get(x, y), newPix;

					switch (quantMode)
					{
					case DitheringQuantiseMode::NBit:
						DitheringQuantise_NBit(oldPix, newPix, bitsPerChannel);
					break;

					case DitheringQuantiseMode::CustomPalette:
						DitheringQuantise_CustomPalette(oldPix, newPix, DITH_PALETTE);
					break;
					}

					output.set(x, y, newPix);

					float errR = oldPix.r - newPix.r;
					float errG = oldPix.g - newPix.g;
					float errB = oldPix.b - newPix.b;

					auto updatePixel = [&](int ox, int oy, size_t i)
					{
						int newX = x + ox;
						int newY = y + oy;

						Pixelf out = input.get(newX, newY);
						out.r += errR * DITHERING_FLOYDSTEINBERG_KERNEL[i];
						out.g += errG * DITHERING_FLOYDSTEINBERG_KERNEL[i];
						out.b += errB * DITHERING_FLOYDSTEINBERG_KERNEL[i];

						output.set(newX, newY, out);
					};

					updatePixel( 1, 0, 0);
					updatePixel(-1, 1, 1);
					updatePixel( 0, 1, 2);
					updatePixel( 1, 1, 3);
				}
		}
		break;

		case Filter::Ripple:
		{
			if (GetKey(def::Key::Z).held) rippleCenterX -= 0.1f * dt;
			if (GetKey(def::Key::X).held) rippleCenterX += 0.1f * dt;
			if (GetKey(def::Key::C).held) rippleCenterY -= 0.1f * dt;
			if (GetKey(def::Key::V).held) rippleCenterY += 0.1f * dt;

			auto lerp = [](float x, float y, float t)
			{
				return x * (1.0f - t) + y * t;
			};

			float fw = (float)FRAME_WIDTH;
			float fh = (float)FRAME_HEIGHT;

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float u = (float)x / fw;
					float v = (float)y / fh;

					float cx = u * rippleCenterX;
					float cy = v * rippleCenterY;

					float l = sqrt(cx * cx + cy * cy);

					float tex_u = u + (cx / l) * lerp(cos(l * 12.0f - accTime * 4.0f) * 0.03f, 0.0f, l / 0.25f);
					float tex_v = v + (cx / l) * lerp(cos(l * 12.0f - accTime * 4.0f) * 0.03f, 0.0f, l / 0.25f);

					output.set(x, y, input.get(int(tex_u * fw), int(tex_v * fh)));
				}
		}
		break;

		}

		accTime += dt;
		if (accTime > 2.0f * 3.14159f) accTime = 0.0f;

		Clear(def::DARK_BLUE);

		DrawFrame(input, 10, 10);
		DrawFrame(output, 470, 10);

		int y = 300;

		switch (filter)
		{
		case Filter::Threshold:
		{
			DrawString(50, y += 16, "Filter: Threshold");
			DrawString(50, y += 16, "Change threshold value with Z and X keys");
			DrawString(50, y += 16, "Current value = " + std::to_string(threshold));
		}
		break;

		case Filter::Motion:
			DrawString(50, y, "Filter: Motion");
		break;

		case Filter::Convolution:
		{
			DrawString(50, y += 16, "Filter: Convolution");
			DrawString(50, y += 16, "Change convolution filter with Z, X, C, V keys");
		}
		break;

		case Filter::LowPass:
		{
			DrawString(50, y += 16, "Filter: LowPass");
			DrawString(50, y += 16, "Change lowpass value with Z and X keys");
			DrawString(50, y += 16, "Current value = " + std::to_string(lowPass));
		}
		break;

		case Filter::Adaptive:
		{
			DrawString(50, y += 16, "Filter: Adaptive Threshold");
			DrawString(50, y += 16, "Change adaptive threshold value with Z and X keys");
			DrawString(50, y += 16, "Current value = " + std::to_string(adaptive));
		}
		break;

		case Filter::Sobel:
			DrawString(50, y, "Filter: Sobel");
		break;

		case Filter::Median:
			DrawString(50, y, "Filter: Median");
		break;

		case Filter::Dithering_FloydSteinberg:
		{
			DrawString(50, y += 16, "Filter: Floyd-Steinberg Dithering");
			DrawString(50, y += 16, "Change bits per channel count with Z and X keys");
			DrawString(50, y += 16, "Change modes with C and V keys");
			DrawString(50, y += 16, "Current value = " + std::to_string(bitsPerChannel));
		}
		break;

		case Filter::Ripple:
			DrawString(50, y += 16, "Filter: Ripple");
			DrawString(50, y += 16, "Change ripple center with Z, X, C, V keys");
			DrawString(50, y += 16, "Current x value = " + std::to_string(rippleCenterX));
			DrawString(50, y += 16, "Current y value = " + std::to_string(rippleCenterY));
		break;

		}

		y = 300;
		DrawString(500, y += 16, "Available filters: ");
		DrawString(500, y += 16, "1) Threshold");
		DrawString(500, y += 16, "2) Motion");
		DrawString(500, y += 16, "3) Convolution");
		DrawString(500, y += 16, "4) LowPass");
		DrawString(500, y += 16, "5) Sobel");
		DrawString(500, y += 16, "6) Median");
		DrawString(500, y += 16, "7) Adaptive");
		DrawString(500, y += 16, "8) Floyd-Steinberg Dithering");
		DrawString(500, y += 16, "9) Ripple");

		return true;
	}

};

int main()
{
	WebCam demo;

	demo.Construct(800, 500, 2, 2);
	demo.Run();

	return 0;
}
