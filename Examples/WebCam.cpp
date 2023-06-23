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

	frame input, output, prevInput;

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
		Median
	};

	Filter filter;

	float fThreshold = 0.5f;
	float fLowPass = 0.1f;
	float fAdaptive = 0.5f;

	float* fConvoKernel = fConvoRidgeKernel;

	float fConvoRidgeKernel[9] =
	{
		0.0f, -1.0f, 0.0f,
		-1.0f, 4.0f, -1.0f,
		0.0f, -1.0f, 0.0f
	};

	float fConvoEdgeKernel[9] =
	{
		-1.0f, -1.0f, -1.0f,
		-1.0f,  8.0f, -1.0f,
		-1.0f, -1.0f, -1.0f
	};

	float fConvoSharpenKernel[9] =
	{
		0.0f, -1.0f, 0.0f,
		-1.0f, 5.0f, -1.0f,
		0.0f, -1.0f, 0.0f
	};

	float fConvoBlurKernel[9] =
	{
		1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
		1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
		1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f
	};

	float fSobelKernelX[9] =
	{
		-1.0f, 0.0f, 1.0f,
		-2.0f, 0.0f, 2.0f,
		-1.0f, 0.0f, 1.0f
	};

	float fSobelKernelY[9] =
	{
		-1.0f, -2.0f, -1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 2.0f, 1.0f
	};

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

		if (GetKey(def::Key::K1).bPressed) filter = Filter::Threshold;
		if (GetKey(def::Key::K2).bPressed) filter = Filter::Motion;
		if (GetKey(def::Key::K3).bPressed) filter = Filter::Convolution;
		if (GetKey(def::Key::K4).bPressed) filter = Filter::LowPass;
		if (GetKey(def::Key::K5).bPressed) filter = Filter::Sobel;
		if (GetKey(def::Key::K6).bPressed) filter = Filter::Median;
		if (GetKey(def::Key::K7).bPressed) filter = Filter::Adaptive;

		switch (filter)
		{
		case Filter::Threshold:
		{
			if (GetKey(def::Key::Z).bHeld) fThreshold -= 0.1f * fDeltaTime;
			if (GetKey(def::Key::X).bHeld) fThreshold += 0.1f * fDeltaTime;

			fThreshold = std::min(1.0f, std::max(fThreshold, 0.0f));

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
					output.set(x, y, input.get(x, y) >= fThreshold ? 1.0f : 0.0f);
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
			if (GetKey(def::Key::Z).bHeld) fConvoKernel = fConvoRidgeKernel;
			if (GetKey(def::Key::X).bHeld) fConvoKernel = fConvoEdgeKernel;
			if (GetKey(def::Key::C).bHeld) fConvoKernel = fConvoSharpenKernel;
			if (GetKey(def::Key::V).bHeld) fConvoKernel = fConvoBlurKernel;

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float fSum = 0.0f;

					for (int i = 0; i < 3; i++)
						for (int j = 0; j < 3; j++)
							fSum += input.get(x + i - 1, y + j - 1) * fConvoKernel[j * 3 + i];

					output.set(x, y, fSum);
				}
		}
		break;

		case Filter::LowPass:
		{
			if (GetKey(def::Key::Z).bHeld) fLowPass -= 0.1f * fDeltaTime;
			if (GetKey(def::Key::X).bHeld) fLowPass += 0.1f * fDeltaTime;

			fLowPass = std::min(1.0f, std::max(fLowPass, 0.0f));

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float fDist = (input.get(x, y) - output.get(x, y)) * fLowPass;
					output.set(x, y, fDist + output.get(x, y));
				}
		}
		break;

		case Filter::Adaptive:
		{
			if (GetKey(def::Key::Z).bHeld) fAdaptive -= 0.1f * fDeltaTime;
			if (GetKey(def::Key::X).bHeld) fAdaptive += 0.1f * fDeltaTime;

			fAdaptive = std::min(1.0f, std::max(fAdaptive, 0.5f));

			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float fSum = 0.0f;

					for (int i = -2; i <= 2; i++)
						for (int j = -2; j <= 2; j++)
							fSum += input.get(x + i, y + j);

					fSum /= 25.0f;
					output.set(x, y, input.get(x, y) > (fSum * fAdaptive) ? 1.0f : 0.0f);
				}
		}
		break;

		case Filter::Sobel:
		{
			for (int y = 0; y < FRAME_HEIGHT; y++)
				for (int x = 0; x < FRAME_WIDTH; x++)
				{
					float fSumX = 0.0f, fSumY = 0.0f;

					for (int i = 0; i < 3; i++)
						for (int j = 0; j < 3; j++)
						{
							fSumX += input.get(x + i - 1, y + j - 1) * fSobelKernelX[j * 3 + i];
							fSumY += input.get(x + i - 1, y + j - 1) * fSobelKernelY[j * 3 + i];
						}

					output.set(x, y, fabs((fSumX + fSumY) / 2.0f));
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

		}

		Clear(def::DARK_BLUE);

		DrawFrame(input, 10, 10);
		DrawFrame(output, 470, 10);

		switch (filter)
		{
		case Filter::Threshold:
		{
			DrawString(50, 300, "Filter: Threshold");
			DrawString(50, 316, "Change threshold value with Z and X keys");
			DrawString(50, 332, "Current value = " + std::to_string(fThreshold));
		}
		break;

		case Filter::Motion:
			DrawString(50, 300, "Filter: Motion");
			break;

		case Filter::Convolution:
		{
			DrawString(50, 300, "Filter: Convolution");
			DrawString(50, 316, "Change convolution filter with Z, X, C, V keys");
		}
		break;

		case Filter::LowPass:
		{
			DrawString(50, 300, "Filter: LowPass");
			DrawString(50, 316, "Change lowpass value with Z and X keys");
			DrawString(50, 332, "Current value = " + std::to_string(fLowPass));
		}
		break;

		case Filter::Adaptive:
		{
			DrawString(50, 300, "Filter: Adaptive Threshold");
			DrawString(50, 316, "Change adaptive threshold value with Z and X keys");
			DrawString(50, 332, "Current value = " + std::to_string(fAdaptive));
		}
		break;

		case Filter::Sobel:
			DrawString(50, 300, "Filter: Sobel");
			break;

		case Filter::Median:
			DrawString(50, 300, "Filter: Median");
			break;

		}

		DrawString(500, 300, "Available filters: ");
		DrawString(500, 316, "1) Threshold");
		DrawString(500, 332, "2) Motion");
		DrawString(500, 348, "3) Convolution");
		DrawString(500, 364, "4) LowPass");
		DrawString(500, 380, "5) Sobel");
		DrawString(500, 396, "6) Median");
		DrawString(500, 412, "7) Adaptive");

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
