#pragma once
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

	void SetTransform(TransformQua transFormQua) {
		world_.transform = transFormQua;
		world_.transform.translate += models_[0]->GetModelData().rootNode.localMatrix.GetTransform();
		if (models_[0]->GetModelData().rootNode.localMatrix.GetScale().x != 1.0f &&
			models_[0]->GetModelData().rootNode.localMatrix.GetScale().y != 1.0f &&
			models_[0]->GetModelData().rootNode.localMatrix.GetScale().z != 1.0f
			) {
			world_.transform.scale += models_[0]->GetModelData().rootNode.localMatrix.GetScale();
		}
		//TODO : levelEditerとの兼ね合いでいったんコメントアウト
		//world_.transform.quaternion = Quaternion::Normalize(models_[0]->GetModelData().rootNode.localMatrix.GetRotation());
		world_.UpdateMatrix();
	};

	uint32_t GetNumber()const { return objectNumber_; };

	WorldTransform& GetWorld() { return world_; };

	std::vector<Model*> Getmodels() { return models_; }

protected:

	std::vector<Model*> models_;
	WorldTransform world_;
	uint32_t objectNumber_ = 0;
	std::string name;
};