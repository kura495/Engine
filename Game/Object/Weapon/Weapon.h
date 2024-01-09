#pragma once

#include "Game/Object/Character.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Input/Input.h"

class Weapon:public BoxCollider
{
public:
	void Initalize(std::vector<Model*> models);
	void Update();
	void Draw(const ViewProjection& viewProj);

	void ImGui();

	void OnCollision(const Collider* collider);

	void SetParent(const WorldTransform& parent);

private:
	std::vector<Model*> models_;
	WorldTransform world_;
};
