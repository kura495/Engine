#pragma once

#include "Game/Object/Character.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Animation/Animation.h"

#include "Audio.h"

class Goal : public Character, public BoxCollider
{
public:

	void Initialize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProjection)override;

	void ImGui();

	void OnCollision(const Collider* collider)override;

private:
	Audio* audio = nullptr;
	Animation* animation;

	float animationTime = 0.0f;
	uint32_t audioHundle = 0;
};

