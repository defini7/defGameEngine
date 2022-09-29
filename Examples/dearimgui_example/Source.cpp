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
	float fFactor = 0.0f;

protected:
	bool OnUserCreate() override
	{
		imgui.Setup(GetWindow(), def::IGT_DARK);

		return true;
	}

	bool OnBeforeUserUpdate(float fDeltaTime) override
	{
		imgui.NewFrame();
		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		Clear(def::WHITE);

		FillRectangle(10, 10, 50, 50, def::Pixel(
			uint8_t(255.0f * fFactor), uint8_t(255.0f * fFactor), uint8_t(255.0f * fFactor)
		));

		ImGui::Begin("Settings");
		ImGui::DragFloat("Factor", &fFactor, 0.01f, 0.0f, 1.0f);
		ImGui::End();

		return true;
	}

	bool OnAfterUserUpdate(float fDeltaTime) override
	{
		imgui.Draw();
		return true;
	}

private:
	def::DearImGui imgui;

};

int main()
{
	Sample demo;

	def::rcode err = demo.Construct(800, 600, 1, 1);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
