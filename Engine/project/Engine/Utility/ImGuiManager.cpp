#include"ImGuiManager.h"

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize(WinApp*winapp,DirectXCommon*directXcommon)
{
	//ImGuiの初期設定
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
	//ImGuiのフレーム開始時の設定
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
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

