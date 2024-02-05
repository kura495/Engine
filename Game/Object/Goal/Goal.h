#pragma once

#include "Game/Object/Character.h"
#include "Base/Utility/BoxCollider.h"

class Goal : public Character, public BoxCollider
{
public:

	void Initialize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProjection)override;

	void ImGui();

	void OnCollision(const Collider* collider)override;

private:

	

};

