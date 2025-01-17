#pragma once
#include "Object/IObject.h"
#include "WorldTransform/WorldTransform.h"
class Emitter
{
public:
	Emitter() {
		world_.Initialize();
	}

	WorldTransform world_;//エミッタのTransform
	uint32_t count = 0;		//発生数
	float frequency = 0;	//発生頻度
	float frequencyTime = 0;//頻度用時刻
	Vector3 particleRadius = {1.0f,1.0f,1.0f};//パーティクルの大きさ
	Vector3 color = {1.0f,1.0f,1.0f};//色
	Vector3 speed = {0.0f,0.0f,0.0f};//速さの倍率
};

