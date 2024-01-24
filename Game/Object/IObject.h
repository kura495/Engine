#pragma once
#include <vector>
#include "Model.h"

class IObject
{
public:
	IObject(){}
	~IObject(){}

	virtual void Initalize(std::vector<Model*> models) = 0;
	virtual void Update() = 0;
	virtual void Draw(const ViewProjection& viewProj) = 0;

	void SetTransform(TransformQua transFormQua) { world_.transform_ = transFormQua;
		world_.UpdateMatrix();};

	uint32_t GetNumber()const { return objectNumber_; };

	WorldTransform& GetWorld() { return world_; };

protected:

	std::vector<Model*> models_;
	WorldTransform world_;
	uint32_t objectNumber_ = 0;
};