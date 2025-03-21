#include "RGBshift.h"
bool RGBshift::isEnableFlag = false;
void RGBshift::Update()
{
	if (isEnableFlag) {
		addEaseT = 0.004f;
		materialData->w = 1.0f;
		materialData->y = materialData->x + random::Generate<float>(-0.002f - easeT, 0.002f + easeT);
		materialData->z = materialData->x + random::Generate<float>(-0.008f - easeT, 0.008f + easeT);
		easeT = (std::max)(easeT - addEaseT, kEaseTMin);
	}
	else {
		Reset();
	}
}

void RGBshift::Reset()
{
	easeT = 0.08f;
	materialData->w = 0.0f;
	materialData->x = 0.0f;
	materialData->y = 0.0f;
	materialData->z = 0.0f;
}
