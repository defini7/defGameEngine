#include "defGameEngine.h"

#define DGE_DEARIMGUI
#include "DGE_DearImGui.h"

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
	float fValue = 0.0f;

protected:
	bool OnUserCreate() override
	{
		imgui.Setup(GetWindow(), def::IGT_DARK);

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		imgui.NewFrame();

		ImGui::Begin("Settings");
		ImGui::Button("Hello");
		ImGui::DragFloat("Value", &fValue);
		ImGui::End();

		Clear(def::WHITE);
		imgui.Draw();

		return true;
	}

private:
	def::DearImGui imgui;

};

int main()
{
	Sample demo;

	def::rcode err = demo.Construct(256, 240, 4, 4);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
