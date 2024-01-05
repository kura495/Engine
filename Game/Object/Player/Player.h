#pragma once

#include "Game/Object/Character.h"
#include "Base/Utility/BoxCollider.h"

class Player : public Character	, public BoxCollider
{
public:

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui()override;

	void OnCollision(const Collider* collider)override;

private:

};


