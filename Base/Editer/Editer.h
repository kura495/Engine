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

	int ObjectCount = 0;
	std::vector<WorldTransform*> world_;
	const ViewProjection* viewProjection_ = nullptr;

	ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
};