#include "RGBshift.h"
void RGBshift::Update()
{
	if (CheckBox) {
		materialData->w = 1.0f;
	}
	else {
		materialData->w = 0.0f;
	}
#ifdef _DEBUG
	ImGui::Begin("PostProsess");
	ImGui::Checkbox("RGBshift", &CheckBox);
	ImGui::DragFloat3("RGBpos", &materialData->x,0.01f);
	ImGui::End();
#endif
}