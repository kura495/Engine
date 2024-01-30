#pragma once

#include "Game/Object/IObject.h"
#include "Base/Utility/BoxCollider.h"

static uint32_t PlaneObjectNumber = 0;

class PlaneObject :public IObject,public BoxCollider
{
public:
	PlaneObject() {
		objectNumber_ = PlaneObjectNumber;
		PlaneObjectNumber++;
	}

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui();

	void OnCollision(const Collider* collider)override;
};

