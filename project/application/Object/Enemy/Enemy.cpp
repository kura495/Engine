#include "Enemy.h"

void Enemy::ImGui()
{
#ifdef USE_IMGUI
	ImGui::Begin(name.c_str());
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x);
	AddImGui();
	ImGui::End();
#endif
}


