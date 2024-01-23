#include"ImGuiManager.h"


ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize(WinApp*winapp,DirectXCommon*directXcommon)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winapp->GetHWND());
	ImGui_ImplDX12_Init(directXcommon->GetDevice(), directXcommon->GetSwapChainDesc().BufferCount, directXcommon->GetrtvDesc().Format,
		directXcommon->GetsrvDescriptorHeap().Get(),
		directXcommon->GetsrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		directXcommon->GetsrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

}

void ImGuiManager::BeginFrame()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGuiIO& io = ImGui::GetIO();

	// Read keyboard modifiers inputs
	io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
	io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
	io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ImGuiManager::GizmoUpdate()
{
	
}

void ImGuiManager::EndFrame()
{
	ImGui::Render();
}
