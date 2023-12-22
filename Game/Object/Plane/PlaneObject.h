#pragma once

#include "Game/Object/IObject.h"

static uint32_t PlaneNumber = 0;

class PlaneObject :public IObject
{
public:
	PlaneObject() {
		objectNumber_ = PlaneNumber;
		PlaneNumber++;
	}

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui()override;
};

