#include "RGBshift.h"
bool RGBshift::isEnableFlag = false;
void RGBshift::Update()
{
	if (isEnableFlag) {
		materialData->w = 1.0f;
		materialData->x = 0.0f;/*random::Generate<float>(-0.002f,0.002f)*/;
		materialData->y = random::Generate<float>(-0.002f - easeT, 0.002f + easeT);
		materialData->z = random::Generate<float>(-0.004f - easeT, 0.004f + easeT);

		easeT = (std::min)(easeT + addEaseT, kEaseTMax);
	}
	else {
		//materialData->w = 1.0f;
		addEaseT = 0.0004f;
		materialData->x = easeT;
		materialData->y = materialData->x - easeT;
		materialData->z = materialData->y - easeT;
		easeT = (std::max)(easeT - addEaseT, 0.0f);
	}

#ifdef _DEBUG
	ImGui::Begin("PostProsess");
	ImGui::Checkbox("RGBshift", &isEnableFlag);
	ImGui::DragFloat3("RGBpos", &materialData->x,0.01f);
	ImGui::End();
#endif
}