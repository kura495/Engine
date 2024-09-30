#include "Enemy.h"

void Enemy::ImGui()
{
#ifdef USE_IMGUI
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x);
	if (ImGui::Button("CollisionOn")) {
		collider.IsUsing = true;
	}
	if (ImGui::Button("CollisionOff")) {
		collider.IsUsing = false;
	}
	ImGui::Text("HP : %d", HP_);
	ImGui::End();
#endif
}

void Enemy::OnCollision(const ICollider* ICollider)
{
	if (ICollider->GetcollitionAttribute() == ColliderTag::Weapon) {
		HP_ -= 1;
		isDamege = true;
		damegeInterval = 0;

	}
	if (ICollider->GetcollitionAttribute() == ColliderTag::Player) {
	}
}

void Enemy::AttackOnCollision(const ICollider* ICollider)
{
	if (ICollider->GetcollitionAttribute() == ColliderTag::Player) {
		attackCollider.IsUsing = false;
	}
}

