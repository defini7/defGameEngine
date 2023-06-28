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

#include <Windows.h>
#include <winternl.h>

#pragma comment(lib,"ntdll.lib")

typedef struct _SYSTEM_PROCESS_INFO
{
	ULONG                   lNextEntryOffset;
	ULONG                   lNumberOfThreads;
	LARGE_INTEGER           nReserved[3];
	LARGE_INTEGER           nCreateTime;
	LARGE_INTEGER           nUserTime;
	LARGE_INTEGER           nKernelTime;
	UNICODE_STRING          sImageName;
	ULONG                   lBasePriority;
	HANDLE                  hProcessId;
	HANDLE                  hInheritedFromProcessId;
} SYSTEM_PROCESS_INFO, *PSYSTEM_PROCESS_INFO;

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Sample");
	}

	~Sample()
	{
		VirtualFree(buffer, 0, MEM_RELEASE);

		imgui.Destroy();
	}

protected:
	bool OnUserCreate() override
	{
		imgui.Setup(GetWindow(), def::IGT_DARK, 10.0f);

		NTSTATUS status;

		buffer = VirtualAlloc(NULL, 1024 * 1024, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (!buffer)
			return false;

		spi = (PSYSTEM_PROCESS_INFO)buffer;

		if (!NT_SUCCESS(status = NtQuerySystemInformation(SystemProcessInformation, spi, 1024 * 1024, NULL)))
		{
			VirtualFree(buffer, 0, MEM_RELEASE);
			return false;
		}

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		imgui.Update();

		while (spi->lNextEntryOffset)
		{
			if (spi->sImageName.Length > 0)
				processes.push_back(spi);

			spi = (PSYSTEM_PROCESS_INFO)((LPBYTE)spi + spi->lNextEntryOffset);
		}

		Clear(def::BLACK);

		ImGui::SetNextWindowSize(ImVec2(ScreenWidth() / 2, ScreenHeight()));

		ImGui::Begin("Processes");

		if (ImGui::BeginTable("Info", 2, ImGuiTableFlags_RowBg + ImGuiTableFlags_Borders))
		{
			ImGui::TableSetupColumn("Id");
			ImGui::TableSetupColumn("Name");
			ImGui::TableHeadersRow();

			for (const auto& p : processes)
			{
				ImGui::TableNextRow();

				ImGui::TableNextColumn();
				ImGui::Text("%d", p->hProcessId);
				ImGui::TableNextColumn();
				ImGui::Text("%ws", p->sImageName.Buffer);
			}
			
			ImGui::EndTable();
		}

		ImGui::End();

		imgui.Draw();

		return true;
	}

private:
	def::DearImGui imgui;

	PVOID buffer;
	PSYSTEM_PROCESS_INFO spi;

	std::list<PSYSTEM_PROCESS_INFO> processes;

};

int main()
{
	Sample demo;

	demo.Construct(800, 600, 1, 1);
	demo.Run();

	return 0;
}
