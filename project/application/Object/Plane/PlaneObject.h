#pragma once
//プレーンオブジェクト//
//オブジェクトマネージャーで使用//
#include "Object/IObject.h"
#include "Collider/OBB/OBBoxCollider.h"
#include <random>
#include <numbers>
#include "Utility/random/random.h"

static uint32_t PlaneObjectNumber = 0;

class PlaneObject :public IObject
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
	void SetPos(Vector3 position) {
		world_.transform.translate = position;
		world_.Update();
	};

private:
	void ImGui();

	OBBoxCollider collider;
	void OnCollision(const ICollider& Collider);

	bool isCrash = false;
	void CrashEffect();
	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float addEaseT = 0.01f;
	//前のフレームのランダムに加算した値を保存
	Vector3 saveramdomTranslate;
};

