#pragma once

#include "Base/Utility/BoxCollider.h"

class SearchPoint:public BoxCollider
{
public:
	void Initalize(WorldTransform* Parent, Vector3 HitBoxSize);
	void Update();
	void Draw();

	void OnCollision(const Collider* collider)override;

	bool GetIsSearchOn() {
		return IsSearchOn;
	}

private:
	bool IsSearchOn = false;
};
