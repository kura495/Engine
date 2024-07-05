#include "PPGrayScale.h"

void PPGrayScale::Update()
{
	if (CheckBox) {
		materialData->x = 1.0f;
	}
	else {
		materialData->x = 0.0f;
	}
	ImGui::Begin("GrayScale");
	ImGui::Checkbox("flag", &CheckBox);
	ImGui::End();
}
