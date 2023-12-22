#pragma once

#include "Game/Object/IObject.h"

static uint32_t BoxNumber = 0;

class BoxObject : public IObject
{
public:
	BoxObject() {
		objectNumber_ = BoxNumber;
		BoxNumber++;
	}

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui()override;
};

