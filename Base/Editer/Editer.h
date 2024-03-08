#pragma once

#include "ImGuiManager.h"
#include "ViewProj/ViewProjection.h"
#include "WorldTransform/WorldTransform.h"
#include "Game/Object/Box/BoxObject.h"
#include "Game/Object/Plane/PlaneObject.h"

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
	void SetWorld(WorldTransform* world) {
		world_.push_back(world);
	}
#pragma endregion 
private:
	Editer() = default;
	~Editer() = default;
	Editer(const Editer& obj) = delete;
	Editer& operator=(const Editer& obj) = delete;

	GlobalVariables* globalVariables = nullptr;

	void GuizmoOption();
	void Manipulator();
	void Grid();
	void ImGuimenu();

	void AddBox();
	void AddPlane();

	int ObjectCount = 0;
	std::vector<WorldTransform*> world_;
	const ViewProjection* viewProjection_ = nullptr;

	bool IsEnableFlag = false;
	bool IsManipulatorFlag = true;
	bool IsGridFlag = true;

	int32_t boxObjectCount;
	int32_t PlaneObjectCount;
	std::vector<Model*> boxModel_;
	std::vector<Model*> planeModel_;

	ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
};