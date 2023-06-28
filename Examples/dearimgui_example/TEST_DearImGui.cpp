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
#include "../../defGameEngine.h"

#define DGE_DEARIMGUI
#include "../../Extensions/DGE_DearImGui.h"

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Sample");
	}

	~Sample()
	{
		imgui.Destroy();
	}

private:
	float factor = 0.0f;

protected:
	bool OnUserCreate() override
	{
		imgui.Setup(GetWindow(), def::IGT_DARK);

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		imgui.Update();

		Clear(def::WHITE);

		FillRectangle(10, 10, 50, 50, def::Pixel(
			uint8_t(255.0f * factor), uint8_t(255.0f * factor), uint8_t(255.0f * factor)
		));

		ImGui::Begin("Settings");
		ImGui::SliderFloat("Factor", &factor, 0.0f, 1.0f);
		ImGui::End();

		imgui.Draw();

		return true;
	}

private:
	def::DearImGui imgui;

};

int main()
{
	Sample demo;

	demo.Construct(800, 600, 1, 1);
	demo.Run();

	return 0;
}
