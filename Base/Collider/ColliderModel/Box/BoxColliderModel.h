#pragma once

#include "Common/DirectX/DirectXCommon.h"
#include "Base/Manager/SRV/SRVManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Base/Object/Model/Model.h"

class BoxColliderModel
{
public:
	void Init();
	void Update();
	void Draw();

	void SetTranslate(Vector3 translate){ world_.transform_.translate = translate; }
	void SetScale(Vector3 scale) {
		world_.transform_.scale = scale;
	}

private:
	DirectXCommon* directX_ = nullptr;
	SRVManager* sRVManager_ = nullptr;

	WorldTransform world_;
	//モデルデータ
	Model* model;
};
