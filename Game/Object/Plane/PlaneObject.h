#pragma once

#include "Object/IObject.h"
#include "Utility/BoxCollider.h"

static uint32_t PlaneObjectNumber = 0;

class PlaneObject :public IObject,public BoxCollider
{
public:
	PlaneObject() {
		objectNumber_ = PlaneObjectNumber;
		PlaneObjectNumber++;
		name = "Plane";
	}

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	void ImGui();

	void OnCollision(const Collider* collider)override;
};

