#pragma once

#pragma region Includes

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#pragma endregion

#ifdef IMGUI_HAS_DOCK
#define DEF_DEARIMGUI_DOCKING
#endif

namespace def
{
	enum ImGuiTheme
	{
		IGT_WHITE,
		IGT_DARK,
		IGT_CLASSIC
	};

	class DearImGui
	{
	public:
		bool Setup(GLFWwindow* window, ImGuiTheme theme, float borderRadius = 0.0f);
		void Update();
		void Draw();
		void Destroy();
	};

#ifdef DGE_DEARIMGUI
#undef DGE_DEARIMGUI

	bool DearImGui::Setup(GLFWwindow* window, ImGuiTheme theme, float borderRadius)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

#ifdef DEF_DEARIMGUI_DOCKING
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
#endif

		// Setup Dear ImGui style
		switch (theme)
		{
		case IGT_WHITE: ImGui::StyleColorsLight(); break;
		case IGT_DARK: ImGui::StyleColorsDark(); break;
		case IGT_CLASSIC: ImGui::StyleColorsClassic(); break;
		}

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = borderRadius;

#ifdef DEF_DEARIMGUI_DOCKING
		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
#endif

		// Setup Platform/Renderer backends
		if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) return false;
		if (!ImGui_ImplOpenGL3_Init("#version 150")) return false;
	}

	void DearImGui::Update()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
	}

	void DearImGui::Draw()
	{
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifdef DEF_DEARIMGUI_DOCKING
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup);
		}
#endif
	}

	void DearImGui::Destroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
#endif
}
