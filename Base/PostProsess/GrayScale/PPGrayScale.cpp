#include "PPGrayScale.h"

void PPGrayScale::Update()
{
	if (CheckBox) {
		materialData->x = 1.0f;
	}
	else {
		materialData->x = 0.0f;
	}
#ifdef _DEBUG

	ImGui::Begin("PostProsess");
	ImGui::Checkbox("GrayScale", &CheckBox);
	ImGui::End();
#endif
}
