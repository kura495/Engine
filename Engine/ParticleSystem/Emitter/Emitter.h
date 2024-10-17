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

};

