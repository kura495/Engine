#pragma once

#include "Common/DirectX/DirectXCommon.h"
#include "Base/Manager/SRV/SRVManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Base/Object/Model/Model.h"

class OBBColliderModel
{
public:
	void Init();
	void Update();
	void Draw();

	void SetTranslate(Vector3 translate) { world_.transform_.translate = translate; }
	void SetScale(Vector3 scale) {
		world_.transform_.scale = scale;
	}
	void SetRotation(Quaternion quaternion) {
		world_.transform_.quaternion = quaternion;
	}
	void SetWorld(Vector3 scale,Quaternion quaternion,Vector3 translate) {
		world_.transform_.translate = translate;
		world_.transform_.quaternion = quaternion;
		world_.transform_.scale = scale;
	}


private:
	DirectXCommon* directX_ = nullptr;
	SRVManager* sRVManager_ = nullptr;

	WorldTransform world_;
	//モデルデータ
	Model* model;
};