#pragma once

#include "Object/IObject.h"
#include "Base/Collider/Box/BoxCollider.h"

static uint32_t BoxObjectNumber = 0;

class BoxObject : public IObject,public BoxCollider
{
public:
	BoxObject() {
		objectNumber_ = BoxObjectNumber;
		BoxObjectNumber++;
		name = "Box";
	}

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	void ImGui();

	void OnCollision(const ICollider* collider)override;

	void SetPos(Vector3 pos) {
		world_.transform_.translate = pos;
	}
};

