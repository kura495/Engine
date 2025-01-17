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

void Enemy::OnCollision(const ICollider& ICollider)
{
	//Weaponと接触していたら
	if (ICollider.GetcollitionAttribute() == ColliderTag::Weapon) {
		HP_ -= 1;
		isDamege = true;
		damegeInterval = 0;

	}
}


