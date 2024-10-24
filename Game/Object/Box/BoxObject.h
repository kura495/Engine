#pragma once
//箱型オブジェクト//
//オブジェクトマネージャーで使用//
#include "Object/IObject.h"
#include "Collider/Box/BoxCollider.h"

static uint32_t BoxObjectNumber = 0;

class BoxObject : public IObject,public BoxCollider
{
public:
	BoxObject() {
		objectNumber_ = BoxObjectNumber;
		BoxObjectNumber++;
		name = "Box";
	}

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	void ImGui();

	void OnCollision(const ICollider* collider);

	void SetPos(Vector3 pos) {
		world_.transform.translate = pos;
	}
};

