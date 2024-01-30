#pragma once

#include "ImGuiManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

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

	void GuizmoOption();
	void Manipulator();
	void Grid();

	int ObjectCount = 0;
	std::vector<WorldTransform*> world_;
	const ViewProjection* viewProjection_ = nullptr;

	bool IsEnableFlag = false;
	bool IsManipulatorFlag = true;
	bool IsGridFlag = true;


	ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
};