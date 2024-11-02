#define DGE_APPLICATION
#include "../../defGameEngine.hpp"

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
