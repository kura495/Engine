#pragma once
#include "Model/Model.h"
#include "WorldTransform/WorldTransform.h"
#include "Collider/OBB/OBBoxCollider.h"
class Sword
{
public:
	void Init();
	void Update();
	void Draw();
private:
	std::unique_ptr<Model>model_;
	WorldTransform world_;
	OBBoxCollider collider_;

	void OnCollision(ICollider& Collider);
};

