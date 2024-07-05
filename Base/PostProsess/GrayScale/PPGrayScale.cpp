#include "PPGrayScale.h"

void PPGrayScale::Update()
{
	if (Player::PushOptionButtern) {
		materialData->x = 1.0f;
	}
	else {
		materialData->x = 0.0f;
	}
	ImGui::Begin("PPGrayScale");
	ImGui::DragFloat("g",&materialData->y);
	ImGui::End();
}
