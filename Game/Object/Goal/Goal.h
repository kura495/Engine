#pragma once

#include "Game/Object/Character.h"
#include "Base/Collider/Box/BoxCollider.h"
#include "Base/Animation/Animation.h"

#include "Audio.h"

class Goal : public Character, public BoxCollider
{
public:

	void Initialize(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	void ImGui();

	void OnCollision(const ICollider* collider);

private:
	Audio* audio = nullptr;
	Animation* animation;

	float animationTime_ = 0.0f;
	uint32_t audioHundle = 0;
};

