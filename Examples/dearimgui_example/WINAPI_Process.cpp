#include "defGameEngine.h"

#define DGE_DEARIMGUI
#include "DGE_DearImGui.h"

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

	bool OnBeforeUserUpdate(float fDeltaTime) override
	{
		imgui.NewFrame();
		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		while (spi->lNextEntryOffset)
		{
			if (spi->sImageName.Length > 0)
				listProcesses.push_back(spi);

			spi = (PSYSTEM_PROCESS_INFO)((LPBYTE)spi + spi->lNextEntryOffset);
		}

		Clear(def::BLACK);

		ImGui::SetNextWindowSize(ImVec2(GetScreenWidth() / 2, GetScreenHeight()));

		ImGui::Begin("Processes");

		if (ImGui::BeginTable("Info", 2, ImGuiTableFlags_RowBg + ImGuiTableFlags_Borders))
		{
			ImGui::TableSetupColumn("Id");
			ImGui::TableSetupColumn("Name");
			ImGui::TableHeadersRow();

			for (const auto& p : listProcesses)
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

		return true;
	}

	bool OnAfterUserUpdate(float fDeltaTime) override
	{
		imgui.Draw();
		return true;
	}

private:
	def::DearImGui imgui;

	PVOID buffer;
	PSYSTEM_PROCESS_INFO spi;

	std::list<PSYSTEM_PROCESS_INFO> listProcesses;

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
