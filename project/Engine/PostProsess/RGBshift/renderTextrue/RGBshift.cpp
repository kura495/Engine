#include "RGBshift.h"
void RGBshift::Update()
{
	if (CheckBox) {
		materialData->x = 1.0f;
	}
	else {
		materialData->x = 0.0f;
	}
#ifdef _DEBUG
	ImGui::Begin("PostProsess");
	ImGui::Checkbox("RGBshift", &CheckBox);
	ImGui::End();
#endif
}