#pragma once
//プレーンオブジェクト//
//オブジェクトマネージャーで使用//
#include "Object/IObject.h"
#include "Collider/Box/BoxCollider.h"

static uint32_t PlaneObjectNumber = 0;

class PlaneObject :public IObject,public BoxCollider
{
public:
	PlaneObject() {
		objectNumber_ = PlaneObjectNumber;
		PlaneObjectNumber++;
		name = "Plane";
	}

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	void ImGui();

	void OnCollision(const ICollider* collider);
};

