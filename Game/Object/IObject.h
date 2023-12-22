#pragma once
#include <vector>
#include "Model.h"



class IObject
{
public:
	IObject() {}

	virtual void Initalize(std::vector<Model*> models) = 0;
	virtual void Update() = 0;
	virtual void Draw(const ViewProjection& viewProj) = 0;

	virtual void ImGui() = 0;

	uint32_t GetNumber()const { return objectNumber_; };

protected:

	std::vector<Model*> models_;
	WorldTransform world_;
	uint32_t objectNumber_ = 0;
};