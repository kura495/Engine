#include "PPGrayScale.h"

void PPGrayScale::Update()
{
	if (CheckBox) {
		materialData->x = 1.0f;
	}
	else {
		materialData->x = 0.0f;
	}
	ImGui::Begin("PostProsess");
	ImGui::Checkbox("GrayScale", &CheckBox);
	ImGui::End();
}
