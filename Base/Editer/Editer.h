#pragma once

#include "ImGuiManager.h"
#include "ViewProj/ViewProjection.h"
#include "WorldTransform/WorldTransform.h"

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
	std::vector<IObject*> object_;
	std::vector<WorldTransform*> world_;
	const ViewProjection* viewProjection_ = nullptr;

	bool IsEnableFlag = false;
	bool IsManipulatorFlag = true;
	bool IsGridFlag = true;

	std::string buf;

	ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
};