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

struct frame
{
	float* pixels = nullptr;

	frame()
	{
		pixels = new float[FRAME_WIDTH * FRAME_HEIGHT];
	}

	~frame()
	{
		delete[] pixels;
	}

	float get(int x, int y)
	{
		if (x < 0 || y < 0 || x >= FRAME_WIDTH || y >= FRAME_HEIGHT)
			return 0.0f;

		return pixels[y * FRAME_WIDTH + x];
	}

	void set(int x, int y, float p)
	{
		if (x < 0 || y < 0 || x >= FRAME_WIDTH || y >= FRAME_HEIGHT)
			return;

		pixels[y * FRAME_WIDTH + x] = p;
	}

	void operator=(const frame& f)
	{
		memcpy(pixels, f.pixels, FRAME_WIDTH * FRAME_HEIGHT * sizeof(float));
	}

};

class WebCam : public def::GameEngine
{
public:
	WebCam()
	{
		SetTitle("Web Cam");
	}

private:
	SimpleCapParams capture;

	frame input, output, prevInput, motionDiffMap;

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
		Dithering_FloydSteinberg
	};

	Filter filter;

	float threshold = 0.5f;
	float lowPass = 0.1f;
	float adaptive = 0.5f;

	const float* convoKernel = CONVO_RIDGE_KERNEL;

	int bitsPerChannel = 3;

protected:
	void DrawFrame(frame& f, int x, int y)
	{
		for (int i = 0; i < FRAME_WIDTH; i++)
			for (int j = 0; j < FRAME_HEIGHT; j++)
			{
				uint8_t c = (uint8_t)std::min(std::max(0.0f, f.pixels[j * FRAME_WIDTH + i] * 255.0f), 255.0f);
				Draw(x + i, y + j, def::Pixel(c, c, c));
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

	bool OnUserUpdate(float fDeltaTime) override
	{
		prevInput = input;
		doCapture(0); while (isCaptureDone(0) == 0);
		for (int y = 0; y < capture.mHeight; y++)
			for (int x = 0; x < capture.mWidth; x++)
			{
				int i = y * capture.mWidth + x;
				RGBint col; col.rgb = capture.mTargetBuf[i];
				input.pixels[i] = (float)col.c[1] / 255.0f;
			}

		if (GetKey(def::Key::K1).pressed) filter = Filter::Threshold;
		if (GetKey(def::Key::K2).pressed) filter = Filter::Motion;
		if (GetKey(def::Key::K3).pressed) filter = Filter::Convolution;
		if (GetKey(def::Key::K4).pressed) filter = Filter::LowPass;
		if (GetKey(def::Key::K5).pressed) filter = Filter::Sobel;
		if (GetKey(def::Key::K6).pressed) filter = Filter::Median;
		if (GetKey(def::Key::K7).pressed) filter = Filter::Adaptive;
		if (GetKey(def::Key::K8).pressed) filter = Filter::Dithering_FloydSteinberg;

		switch (filter)
		{
		case Filter::Threshold:
		{
			if (GetKey(def::Key::Z).held) threshold -= 0.1f * fDeltaTime;
			if (GetKey(def::Key::X).held) threshold += 0.1f * fDeltaTime;

			threshold = std::min(1.0f, std::max(threshold, 0.0f));

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
					output.set(x, y, input.get(x, y) >= threshold ? 1.0f : 0.0f);
		}
		break;

		case Filter::Motion:
		{
			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
					output.set(x, y, fabs(input.get(x, y) - prevInput.get(x, y)));
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
					float fSum = 0.0f;

					for (int i = 0; i < 3; i++)
						for (int j = 0; j < 3; j++)
							fSum += input.get(x + i - 1, y + j - 1) * convoKernel[j * 3 + i];

					output.set(x, y, fSum);
				}
		}
		break;

		case Filter::LowPass:
		{
			if (GetKey(def::Key::Z).held) lowPass -= 0.1f * fDeltaTime;
			if (GetKey(def::Key::X).held) lowPass += 0.1f * fDeltaTime;

			lowPass = std::min(1.0f, std::max(lowPass, 0.0f));

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float dist = (input.get(x, y) - output.get(x, y)) * lowPass;
					output.set(x, y, dist + output.get(x, y));
				}
		}
		break;

		case Filter::Adaptive:
		{
			if (GetKey(def::Key::Z).held) adaptive -= 0.1f * fDeltaTime;
			if (GetKey(def::Key::X).held) adaptive += 0.1f * fDeltaTime;

			adaptive = std::min(1.0f, std::max(adaptive, 0.5f));

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float sum = 0.0f;

					for (int i = -2; i <= 2; i++)
						for (int j = -2; j <= 2; j++)
							sum += input.get(x + i, y + j);

					sum /= 25.0f;
					output.set(x, y, input.get(x, y) > (sum * adaptive) ? 1.0f : 0.0f);
				}
		}
		break;

		case Filter::Sobel:
		{
			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float sumX = 0.0f, sumY = 0.0f;

					for (int i = 0; i < 3; i++)
						for (int j = 0; j < 3; j++)
						{
							float col = input.get(x + i - 1, y + j - 1);
							sumX += col * SOBEL_KERNEL_X[j * 3 + i];
							sumY += col * SOBEL_KERNEL_Y[j * 3 + i];
						}

					output.set(x, y, fabs((sumX + sumY) / 2.0f));
				}
		}
		break;

		case Filter::Median:
		{
			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					std::array<float, 25> col;

					for (int i = 0; i < 5; i++)
						for (int j = 0; j < 5; j++)
							col[j * 5 + i] = input.get(x + i - 2, y + j - 2);

					std::sort(col.begin(), col.end(), [](float c1, float c2) { return c2 < c1; });
					output.set(x, y, col[12]);
				}

		}
		break;

		case Filter::Dithering_FloydSteinberg:
		{
			if (GetKey(def::Key::Z).pressed) bitsPerChannel = std::max(bitsPerChannel - 1, 1);
			if (GetKey(def::Key::X).pressed) bitsPerChannel = std::min(bitsPerChannel + 1, 31);

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float oldPix = input.get(x, y);
					float levels = (1 << bitsPerChannel) - 1;
					float newPix = std::clamp(round(oldPix * levels) / levels, 0.0f, 1.0f);

					output.set(x, y, newPix);

					float err = oldPix - newPix;

					auto updatePixel = [&](int ox, int oy, size_t i)
					{
						int newX = x + ox;
						int newY = y + oy;
						output.set(newX, newY, input.get(newX, newY) + err * DITHERING_FLOYDSTEINBERG_KERNEL[i]);
					};

					updatePixel( 1, 0, 0);
					updatePixel(-1, 1, 1);
					updatePixel( 0, 1, 2);
					updatePixel( 1, 1, 3);
				}
		}
		break;

		}

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
			DrawString(50, y += 16, "Filter: Floyd-Steinberg Dithering");
			DrawString(50, y += 16, "Change bits per channel count with Z and X keys");
			DrawString(50, y += 16, "Current value = " + std::to_string(bitsPerChannel));
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
