#include "WorkShake.h"

void WorkShake::Init(float Time, Vector2 ShakeValue)
{
	easeT = 0.0f;
	flag = true;
	timer = Time;
	minShakeValue = ShakeValue.x;
	maxShakeValue = ShakeValue.y;
}

void WorkShake::Update(Vector3& translate)
{
	if (flag) {
		easeT = (std::min)(easeT + kDeltaTime, timer);

		Vector3 ramdomTranslate = { random::Generate<float>(minShakeValue,maxShakeValue),random::Generate<float>(minShakeValue,maxShakeValue),random::Generate<float>(minShakeValue, maxShakeValue) };

		translate += ramdomTranslate;

		if (easeT == timer) {
			flag = false;
		}
	}
}
