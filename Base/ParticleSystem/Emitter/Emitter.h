#pragma once
#include "Base/Editer/Editer.h"
#include "WorldTransform/WorldTransform.h"
class Emitter
{
public:
	Emitter() {
		world_.Initialize();
		//Editer::GetInstance()->SetWorld(&world_);
	}

	WorldTransform world_;//エミッタのTransform
	uint32_t count = 0;		//発生数
	float frequency = 0;	//発生頻度
	float frequencyTime = 0;//頻度用時刻

};

