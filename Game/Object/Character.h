#pragma once

#include <vector>
#include "Object/Model/Model.h"


class Character
{
public:

	virtual void Initialize(std::vector<Model*> models) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:

	std::vector<Model*> models_;
	WorldTransform world_;

};
