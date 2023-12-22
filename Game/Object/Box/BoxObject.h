#pragma once

#include "Game/Object/IObject.h"

class BoxObject : public IObject
{
public:

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui()override;
};

