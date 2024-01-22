#pragma once

#include <vector>
#include "Model.h"


class Character
{
public:

	virtual void Initialize(std::vector<Model*> models) = 0;
	virtual void Update() = 0;
	virtual void Draw(const ViewProjection& viewProj) = 0;

	virtual void ImGui() = 0;

protected:

	std::vector<Model*> models_;
	WorldTransform world_;

};
