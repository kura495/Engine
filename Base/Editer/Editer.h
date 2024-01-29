#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class Editer {
public:
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
	void GuizmoOption();
	void Manipulator();

	uint32_t ObjectCount = 0;
	std::vector<WorldTransform*> world_;
	const ViewProjection* viewProjection_ = nullptr;
};