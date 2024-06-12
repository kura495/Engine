#pragma once

#include "ImGuiManager.h"
#include "ViewProj/ViewProjection.h"
#include "WorldTransform/WorldTransform.h"

#include "Base/Common/WinApp/WinApp.h"
#include "Renderer/Renderer.h"
class IObject;
class Editer {
public:
	static Editer* GetInstance();

	void Initalize();
	void Update();
	void Draw();

	void IsEnable(bool Flag) {
		IsEnableFlag = Flag;
	}


#pragma region Setter
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	void SetObject(IObject* object);
#pragma endregion
private:
	Editer() = default;
	~Editer() = default;
	Editer(const Editer& obj) = delete;
	Editer& operator=(const Editer& obj) = delete;

	void GuizmoOption();
	void Manipulator();
	void Grid();

	int ObjectCount = 0;
	std::vector<IObject*> object_;
	const ViewProjection* viewProjection_ = nullptr;
	std::string textrueName;

	bool IsEnableFlag = false;
	bool IsManipulatorFlag = true;
	bool IsGridFlag = false;

	ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
};