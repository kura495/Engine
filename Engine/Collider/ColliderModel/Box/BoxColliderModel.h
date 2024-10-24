#pragma once
//コライダー表示用モデル//
//AABB用//
#include "Common/DirectX/DirectXCommon.h"
#include "Manager/SRV/SRVManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Object/Model/Model.h"

class BoxColliderModel
{
public:
	void Init();
	void Update();
	void Draw();

	void SetTranslate(Vector3 translate){ world_.transform.translate = translate; }
	void SetScale(Vector3 scale) {
		world_.transform.scale = scale;
	}

private:
	DirectXCommon* directX_ = nullptr;
	SRVManager* sRVManager_ = nullptr;

	WorldTransform world_;
	//モデルデータ
	Model* model;
};
