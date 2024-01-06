#pragma once

#include "Game/Object/IObject.h"

static uint32_t PlaneObjectNumber = 0;

class PlaneObject :public IObject
{
public:
	PlaneObject() {
		objectNumber_ = PlaneObjectNumber;
		PlaneObjectNumber++;
	}

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui()override;
};

