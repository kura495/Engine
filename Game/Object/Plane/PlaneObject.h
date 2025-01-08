#pragma once
//プレーンオブジェクト//
//オブジェクトマネージャーで使用//
#include "Object/IObject.h"
#include "Collider/OBB/OBBoxCollider.h"

static uint32_t PlaneObjectNumber = 0;

class PlaneObject :public IObject
{
public:
	PlaneObject() {
		objectNumber_ = PlaneObjectNumber;
		PlaneObjectNumber++;
		name = "Plane";
	}

	OBBoxCollider collider;

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;
	void SetPos(Vector3 position) {
		world_.transform.translate = position;
		world_.Update();
	};

private:
	void ImGui();

	void OnCollision(const ICollider& Collider);

	bool isCrash;
	void CrashEffect();
};

