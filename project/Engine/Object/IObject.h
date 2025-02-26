#pragma once
//オブジェクトの基底クラス//
#include <vector>
#include "Object/Model/Model.h"
#include "Editer/Editer.h"

static uint32_t ObjectNumber = 0;

class IObject
{
public:
	IObject(){
		Editer::GetInstance()->SetObject(this);
		objectNumber_ = ObjectNumber;
		ObjectNumber++;
	}
	~IObject(){}
	
	virtual void Init(std::vector<Model*> models);
	virtual void Update();
	virtual void Draw();

#pragma region
	uint32_t GetNumber()const { return objectNumber_; };

	const WorldTransform& GetWorld() { return world_; };

	std::vector<Model*> Getmodels() { return models_; }
#pragma endregion Getter

protected:

	std::vector<Model*> models_;
	WorldTransform world_;
	uint32_t objectNumber_ = 0;
	std::string name;
};