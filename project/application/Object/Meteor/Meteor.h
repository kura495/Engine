#pragma once
#include "Model/Model.h"
#include "Utility/GlobalTime.h"
#include "Utility/random/random.h"
class Meteor
{
public:
	void Init();
	void Update();
	void Draw();
private:
	//モデル
	std::unique_ptr<Model> model_;
	WorldTransform world_;
};
