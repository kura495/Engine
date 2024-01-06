#pragma once

#include "Game/Object/IObject.h"

static uint32_t BoxObjectNumber = 0;

class BoxObject : public IObject
{
public:
	BoxObject() {
		objectNumber_ = BoxObjectNumber;
		BoxObjectNumber++;
	}

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui()override;
};

