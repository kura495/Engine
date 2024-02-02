#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"


class ImGuiManager
{
public:
	static ImGuiManager* GetInstance();
	void Initialize(WinApp* winapp,DirectXCommon*directXcommon);
	void BeginFrame();
	void GizmoUpdate();
	void EndFrame();
	void EditTransform(Matrix4x4* cameraView,Matrix4x4* cameraProjection,Matrix4x4* matrix);
private:
	ImGuiManager() = default;
	~ImGuiManager() = default;
	ImGuiManager(const ImGuiManager& obj) = delete;
	ImGuiManager& operator=(const ImGuiManager& obj) = delete;


};