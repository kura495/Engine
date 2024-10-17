#pragma once

#include "Common/DirectX/DirectXCommon.h"
#include "Manager/SRV/SRVManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Object/Model/Model.h"

class OBBColliderModel
{
public:
	void Init();
	void Update();
	void Draw();

	void SetTranslate(Vector3 translate) { world_.transform.translate = translate; }
	void SetScale(Vector3 scale) {
		world_.transform.scale = scale;
	}
	void SetRotation(Quaternion quaternion) {
		world_.transform.quaternion = quaternion;
	}
	void SetWorld(Vector3 scale,Quaternion quaternion,Vector3 translate) {
		world_.transform.translate = translate;
		world_.transform.quaternion = quaternion;
		world_.transform.scale = scale;
	}


private:
	DirectXCommon* directX_ = nullptr;
	SRVManager* sRVManager_ = nullptr;

	WorldTransform world_;
	//モデルデータ
	Model* model;
};