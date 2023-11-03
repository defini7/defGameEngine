/*
*	BSD 3-Clause License
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
#include "defGameEngine.hpp"
#include "escapi.h"

#define DEF_IMAGE_PROCESSING
#include "ImageProcessing.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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
		def::img::Pixelf d;
		d.r = s.r - in.r;
		d.g = s.g - in.g;
		d.b = s.b - in.b;

		float dist = sqrt(d.r * d.r + d.g * d.g + d.b * d.b);

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

	virtual ~WebCam()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
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
		Ripple,
		CRT
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
	float rippleCenter[2] = { 0.5f, 0.5f };

	// Windows Command Prompt colors by default
	std::vector<def::img::Pixelf> vDitheringPalette =
	{
		{ 12.0f / 255.0f,12.0f / 255.0f, 12.0f / 255.0f },
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
	
	def::img::Pixelf crtGun = { 0.0f, 0.0f, 0.0f };
	def::img::Pixelf crtSkew = { 0.64f, 0.24f, 0.4f };
	def::img::Pixelf crtAlign = { 1.1f, -0.8f, 0.2f };

	bool convoRadio[4] = { true };
	bool dithFloydRadio[2] = { true };
	bool dithOrdRadio[2] = { true };

	bool textEntering = false;

	def::img::Pixelf customColor;
	int customCursor = 0;

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

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		if (!ImGui_ImplGlfw_InitForOpenGL(GetWindow(), true)) return false;
		if (!ImGui_ImplOpenGL3_Init("#version 130")) return false;

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

		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		if (!textEntering)
		{
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
			if (GetKey(def::Key::MINUS).pressed) filter = Filter::CRT;

			if (GetKey(def::Key::SPACE).held)
				def::img::Greyscale(input, input);
		}

		Clear(def::DARK_BLUE);

		auto press_radio = [](const char* label, bool* values, size_t count, size_t i)
		{
			if (ImGui::RadioButton(label, values[i]))
			{
				values[i] = true;
				for (size_t v = 0; v < count; v++)
				{
					if (i != v)
						values[v] = false;
				}

				return true;
			}

			return false;
		};

		switch (filter)
		{
		case Filter::Threshold:
		{
			ImGui::SliderFloat("Value", &threshold, 0.0f, 1.0f);
			def::img::Threshold(input, output, threshold);
		}
		break;

		case Filter::Convolution:
		{
			if (press_radio("Ridge", convoRadio, 4, 0)) convoKernel = def::img::CONVO_RIDGE_KERNEL;
			if (press_radio("Edge", convoRadio, 4, 1)) convoKernel = def::img::CONVO_EDGE_KERNEL;
			if (press_radio("Sharpen", convoRadio, 4, 2)) convoKernel = def::img::CONVO_SHARPEN_KERNEL;
			if (press_radio("Blur", convoRadio, 4, 3)) convoKernel = def::img::CONVO_BLUR_KERNEL;

			def::img::Convolution(input, output, convoKernel);
		}
		break;

		case Filter::LowPass:
		{
			ImGui::SliderFloat("Value", &lowPass, 0.0f, 1.0f);
			def::img::LowPass(input, output, lowPass);
		}
		break;

		case Filter::Adaptive:
		{
			ImGui::SliderFloat("Value", &adaptive, 0.5f, 1.0f);
			def::img::Adaptive(input, output, adaptive);
		}
		break;

		case Filter::Sobel: def::img::Sobel(input, output); break;
		case Filter::Median: def::img::Median(input, output); break;
		case Filter::Motion: def::img::Motion(input, prevInput, output); break;

		case Filter::Dithering_FloydSteinberg:
		{
			if (press_radio("NBit", dithFloydRadio, 2, 0)) quantMode = DitheringQuantiseMode::NBit;
			if (press_radio("Custom", dithFloydRadio, 2, 1)) quantMode = DitheringQuantiseMode::CustomPalette;

			ImGui::SliderInt("Bits per channel", &bitsPerChannel, 1, 8);

			textEntering = (quantMode == DitheringQuantiseMode::CustomPalette);

			if (textEntering)
			{
				ImGui::InputFloat3("New color", customColor.rgb);
				if (ImGui::Button("Add")) vDitheringPalette.push_back(customColor);
				if (ImGui::Button("Remove")) vDitheringPalette.erase(vDitheringPalette.begin() + customCursor);

				auto get_item = [](void* data, int idx, const char** output) -> bool
				{
					*output = ((def::img::Pixelf*)data)[idx].str().c_str();
					return true;
				};

				ImGui::ListBox("Palette", &customCursor, get_item, vDitheringPalette.data(), (int)vDitheringPalette.size());
			}

			def::img::Dithering_FloydSteinberg(input, output,
				[&](const def::img::Pixelf& in, def::img::Pixelf& out) {
					switch (quantMode)
					{
					case DitheringQuantiseMode::NBit:
						DitheringQuantise_NBit(in, out, bitsPerChannel);
						break;

					case DitheringQuantiseMode::CustomPalette:
						DitheringQuantise_CustomPalette(in, out, vDitheringPalette);
						break;
					}
				});
		}
		break;

		case Filter::Dithering_Ordered:
		{
			ImGui::Text("Matrix Type");

			if (press_radio("2x2", dithOrdRadio, 2, 0)) matrixMode = DitheringMatrixSizeMode::Two;
			if (press_radio("4x4", dithOrdRadio, 2, 1)) matrixMode = DitheringMatrixSizeMode::Four;

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
			ImGui::SliderFloat2("Center", rippleCenter, 0.0f, 1.0f);
			def::img::Ripple(input, output, accTime, rippleCenter[0], rippleCenter[1]);
		}
		break;

		case Filter::CRT:
		{
			ImGui::SliderFloat3("Gun", crtGun.rgb, -10.0f, 10.0f);
			ImGui::SliderFloat3("Skew", crtSkew.rgb, -10.0f, 10.0f);
			ImGui::SliderFloat3("Align", crtAlign.rgb, -10.0f, 10.0f);

			def::img::CRT(input, output, crtGun, crtSkew, crtAlign);
		}
		break;

		}

		ImGui::EndFrame();

		accTime += dt;
		if (accTime > 2.0f * 3.14159f) accTime = 0.0f;

		DrawFrame(input, 10, 10);
		DrawFrame(output, 470, 10);

		int y = 300;

		switch (filter)
		{
		case Filter::Threshold:
			DrawString(50, y, "Filter: Threshold");
			break;

		case Filter::Motion:
			DrawString(50, y, "Filter: Motion");
			break;

		case Filter::Convolution:
			DrawString(50, y, "Filter: Convolution");
			break;

		case Filter::LowPass:
			DrawString(50, y, "Filter: LowPass");
			break;

		case Filter::Adaptive:
			DrawString(50, y, "Filter: Adaptive Threshold");
			break;

		case Filter::Sobel:
			DrawString(50, y, "Filter: Sobel");
			break;

		case Filter::Median:
			DrawString(50, y, "Filter: Median");
			break;

		case Filter::Dithering_FloydSteinberg:
			DrawString(50, y, "Filter: Floyd-Steinberg Dithering");
			break;

		case Filter::Dithering_Ordered:
			DrawString(50, y, "Filter: Ordered Dithering");
			break;

		case Filter::Ripple:
			DrawString(50, y, "Filter: Ripple");
			break;

		case Filter::CRT:
			DrawString(50, y, "Filter: CRT");
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
		DrawString(500, y += 16, "-) CRT");

		return true;
	}

	bool OnAfterDraw() override
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
