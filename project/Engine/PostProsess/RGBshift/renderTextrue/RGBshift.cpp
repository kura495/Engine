#include "RGBshift.h"
bool RGBshift::isEnableFlag = false;
void RGBshift::Update()
{
	if (isEnableFlag) {
		addEaseT = 0.0005f;
		materialData->w = 1.0f;
		materialData->x = 0.0f;
		materialData->y = materialData->x + random::Generate<float>(-0.002f - easeT, 0.002f + easeT);
		materialData->z = materialData->x + random::Generate<float>(-0.008f - easeT, 0.008f + easeT);

		easeT = (std::min)(easeT + addEaseT, kEaseTMax);
	}
	else {
		addEaseT = 0.001f;
		materialData->x = 0.0f;
		materialData->y = materialData->x - easeT;
		materialData->z = materialData->y - easeT;
		easeT = (std::max)(easeT - addEaseT, kEaseTMin);

		if (easeT == kEaseTMin) {
			materialData->w = 0.0f;
		}
	}

#ifdef _DEBUG
	ImGui::Begin("PostProsess");
	ImGui::Checkbox("RGBshift", &isEnableFlag);
	ImGui::DragFloat3("RGBpos", &materialData->x,0.01f);
	ImGui::DragFloat("easeT", &easeT,0.01f);

	ImGui::End();
#endif
}