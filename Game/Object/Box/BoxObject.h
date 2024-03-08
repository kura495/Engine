#pragma once

#include "Object/IObject.h"
#include "Utility/BoxCollider.h"

static uint32_t BoxObjectNumber = 0;

class BoxObject : public IObject,public BoxCollider
{
public:
	BoxObject() {
		objectNumber_ = BoxObjectNumber;
		BoxObjectNumber++;
	}

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui();

	void OnCollision(const Collider* collider)override;

	void SetPos(Vector3 pos) {
		world_.transform_.translate = pos;
	}
};

