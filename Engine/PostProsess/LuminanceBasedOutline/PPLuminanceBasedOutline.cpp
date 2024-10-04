#include "PPLuminanceBasedOutline.h"

void PPLuminanceBasedOutline::Update()
{
	if (CheckBox) {
		materialData->x = 1.0f;
	}
	else {
		materialData->x = 0.0f;
	}
#ifdef _DEBUG
	ImGui::Begin("PostProsess");
	ImGui::Checkbox("LuminanceBasedOutline", &CheckBox);
	ImGui::End();
#endif
}
