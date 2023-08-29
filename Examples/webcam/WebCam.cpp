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
#include "defGameEngine.h"
#include "escapi.h"

#define DEF_IMAGE_PROCESSING
#include "ImageProcessing.hpp"

constexpr int FRAME_WIDTH = 320;
constexpr int FRAME_HEIGHT = 240;

void DitheringQuantise_NBit(const def::img::Pixelf& in, def::img::Pixelf& out, const int bitsPerChannel)
{
	float levels = (1 << bitsPerChannel) - 1;
	out.r = std::clamp(round(in.r * levels) / levels, 0.0f, 1.0f);
	out.g = std::clamp(round(in.g * levels) / levels, 0.0f, 1.0f);
	out.b = std::clamp(round(in.b * levels) / levels, 0.0f, 1.0f);
}

void DitheringQuantise_CustomPalette(const def::img::Pixelf& in, def::img::Pixelf& out, const std::vector<def::img::Pixelf>& shades)
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

	~WebCam()
	{
		deinitCapture(0);
	}

private:
	SimpleCapParams capture;

	def::img::Frame input, output, prevInput;

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
		Dithering_Ordered,
		Ripple
	};

	enum class DitheringQuantiseMode
	{
		NBit,
		CustomPalette
	};

	enum class DitheringMatrixSizeMode
	{
		Two,
		Four
	};

	Filter filter = Filter::Threshold;
	DitheringQuantiseMode quantMode = DitheringQuantiseMode::NBit;
	DitheringMatrixSizeMode matrixMode = DitheringMatrixSizeMode::Two;

	float threshold = 0.5f;
	float lowPass = 0.1f;
	float adaptive = 0.5f;

	const float* convoKernel = def::img::CONVO_RIDGE_KERNEL;

	int bitsPerChannel = 3;

	float accTime = 0.0f;
	float rippleCenterX = 0.5f;
	float rippleCenterY = 0.5f;

	// Windows Command Prompt colors
	const std::vector<def::img::Pixelf> DITH_PALETTE = {
		{ 12.0f / 255.0f,12.0f / 255.0f,12.0f / 255.0f },
		{ 0.0f, 55.0f / 255.0f, 218.0f / 255.0f },
		{ 19.0f / 255.0f, 161.0f / 255.0f, 14.0f / 255.0f },
		{ 58.0f / 255.0f, 150.0f / 255.0f, 221.0f / 255.0f },
		{ 197.0f / 255.0f, 15.0f / 255.0f, 31.0f / 255.0f },
		{ 136.0f / 255.0f, 23.0f / 255.0f, 152.0f / 255.0f },
		{ 193.0f / 255.0f, 156.0f / 255.0f, 0.0f },
		{ 204.0f / 255.0f, 204.0f / 255.0f, 204.0f / 255.0f },
		{ 118.0f / 255.0f, 118.0f / 255.0f, 118.0f / 255.0f },
		{ 59.0f / 255.0f, 120.0f / 255.0f, 1.0f },
		{ 22.0f / 255.0f, 198.0f / 255.0f, 12.0f / 255.0f },
		{ 97.0f / 255.0f, 214.0f / 255.0f, 214.0f / 255.0f },
		{ 231.0f / 255.0f, 72.0f / 255.0f, 86.0f / 255.0f },
		{ 180.0f / 255.0f, 0.0f, 158.0f / 255.0f },
		{ 249.0f / 255.0f, 241.0f / 255.0f, 165.0f / 255.0f },
		{ 242.0f / 255.0f, 242.0f / 255.0f, 242.0f / 255.0f, },
	};

protected:
	void DrawFrame(def::img::Frame& f, int x, int y)
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
		if (initCapture(0, &capture) == 0) return false;

		input.create(FRAME_WIDTH, FRAME_HEIGHT);
		prevInput.create(FRAME_WIDTH, FRAME_HEIGHT);
		output.create(FRAME_WIDTH, FRAME_HEIGHT);

		return true;
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
		if (GetKey(def::Key::K9).pressed) filter = Filter::Dithering_Ordered;
		if (GetKey(def::Key::K0).pressed) filter = Filter::Ripple;

		if (GetKey(def::Key::SPACE).held)
			def::img::Greyscale(input, input);

		switch (filter)
		{
		case Filter::Threshold:
		{
			if (GetKey(def::Key::Z).held) threshold -= 0.1f * dt;
			if (GetKey(def::Key::X).held) threshold += 0.1f * dt;

			threshold = std::min(1.0f, std::max(threshold, 0.0f));

			Threshold(input, output, threshold);
		}
		break;

		case Filter::Convolution:
		{
			if (GetKey(def::Key::Z).held) convoKernel = def::img::CONVO_RIDGE_KERNEL;
			if (GetKey(def::Key::X).held) convoKernel = def::img::CONVO_EDGE_KERNEL;
			if (GetKey(def::Key::C).held) convoKernel = def::img::CONVO_SHARPEN_KERNEL;
			if (GetKey(def::Key::V).held) convoKernel = def::img::CONVO_BLUR_KERNEL;

			Convolution(input, output, convoKernel);
		}
		break;

		case Filter::LowPass:
		{
			if (GetKey(def::Key::Z).held) lowPass -= 0.1f * dt;
			if (GetKey(def::Key::X).held) lowPass += 0.1f * dt;

			lowPass = std::min(1.0f, std::max(lowPass, 0.0f));

			LowPass(input, output, lowPass);
		}
		break;

		case Filter::Adaptive:
		{
			if (GetKey(def::Key::Z).held) adaptive -= 0.1f * dt;
			if (GetKey(def::Key::X).held) adaptive += 0.1f * dt;

			adaptive = std::min(1.0f, std::max(adaptive, 0.5f));

			Adaptive(input, output, adaptive);
		}
		break;

		case Filter::Sobel: Sobel(input, output); break;
		case Filter::Median: Median(input, output); break;
		case Filter::Motion: Motion(input, prevInput, output); break;

		case Filter::Dithering_FloydSteinberg:
		{
			if (GetKey(def::Key::Z).pressed) bitsPerChannel--;
			if (GetKey(def::Key::X).pressed) bitsPerChannel++;
			if (GetKey(def::Key::C).pressed) quantMode = DitheringQuantiseMode::NBit;
			if (GetKey(def::Key::V).pressed) quantMode = DitheringQuantiseMode::CustomPalette;

			bitsPerChannel = std::min(std::max(bitsPerChannel, 1), 8);

			Dithering_FloydSteinberg(input, output,
				[&](const def::img::Pixelf& in, def::img::Pixelf& out) {
					switch (quantMode)
					{
					case DitheringQuantiseMode::NBit:
						DitheringQuantise_NBit(in, out, bitsPerChannel);
						break;

					case DitheringQuantiseMode::CustomPalette:
						DitheringQuantise_CustomPalette(in, out, DITH_PALETTE);
						break;
					}
				});
		}
		break;

		case Filter::Dithering_Ordered:
		{
			if (GetKey(def::Key::Z).pressed) matrixMode = DitheringMatrixSizeMode::Two;
			if (GetKey(def::Key::X).pressed) matrixMode = DitheringMatrixSizeMode::Four;

			switch (matrixMode)
			{
			case DitheringMatrixSizeMode::Two:
				def::img::Dithering_Ordered(input, output, def::img::DITHERING_ORDERED_2X2, 2, 2);
				break;

			case DitheringMatrixSizeMode::Four:
				def::img::Dithering_Ordered(input, output, def::img::DITHERING_ORDERED_4X4, 4, 4);
				break;
			}
		}
		break;

		case Filter::Ripple:
		{
			if (GetKey(def::Key::Z).held) rippleCenterX -= 0.1f * dt;
			if (GetKey(def::Key::X).held) rippleCenterX += 0.1f * dt;
			if (GetKey(def::Key::C).held) rippleCenterY -= 0.1f * dt;
			if (GetKey(def::Key::V).held) rippleCenterY += 0.1f * dt;

			Ripple(input, output, accTime, rippleCenterX, rippleCenterY);
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
			DrawString(50, y, "Filter: Threshold");
			DrawString(50, y += 16, "Change threshold value with Z and X keys");
			DrawString(50, y += 16, "Current value = " + std::to_string(threshold));
		}
		break;

		case Filter::Motion:
			DrawString(50, y, "Filter: Motion");
			break;

		case Filter::Convolution:
		{
			DrawString(50, y, "Filter: Convolution");
			DrawString(50, y += 16, "Change convolution filter with Z, X, C, V keys");
		}
		break;

		case Filter::LowPass:
		{
			DrawString(50, y, "Filter: LowPass");
			DrawString(50, y += 16, "Change lowpass value with Z and X keys");
			DrawString(50, y += 16, "Current value = " + std::to_string(lowPass));
		}
		break;

		case Filter::Adaptive:
		{
			DrawString(50, y, "Filter: Adaptive Threshold");
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
			DrawString(50, y, "Filter: Floyd-Steinberg Dithering");
			DrawString(50, y += 16, "Change bits per channel count with Z and X keys");
			DrawString(50, y += 16, "Change modes with C and V keys");
			DrawString(50, y += 16, "Current value = " + std::to_string(bitsPerChannel));
		}
		break;

		case Filter::Dithering_Ordered:
		{
			DrawString(50, y, "Filter: Ordered Dithering");
			DrawString(50, y += 16, "Change mode with Z, X keys");
		}
		break;

		case Filter::Ripple:
			DrawString(50, y, "Filter: Ripple");
			DrawString(50, y += 16, "Change ripple center with Z, X, C, V keys");
			DrawString(50, y += 16, "Current x value = " + std::to_string(rippleCenterX));
			DrawString(50, y += 16, "Current y value = " + std::to_string(rippleCenterY));
			break;

		}

		y = 300;
		DrawString(500, y, "Available filters: ");
		DrawString(500, y += 16, "1) Threshold");
		DrawString(500, y += 16, "2) Motion");
		DrawString(500, y += 16, "3) Convolution");
		DrawString(500, y += 16, "4) LowPass");
		DrawString(500, y += 16, "5) Sobel");
		DrawString(500, y += 16, "6) Median");
		DrawString(500, y += 16, "7) Adaptive");
		DrawString(500, y += 16, "8) Floyd-Steinberg Dithering");
		DrawString(500, y += 16, "9) Ordered Dithering");
		DrawString(500, y += 16, "0) Ripple");

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
