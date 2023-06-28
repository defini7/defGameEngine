#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, 2022, 2023 Alex
	All rights reserved.
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
***/
#pragma endregion

#pragma region includes

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
