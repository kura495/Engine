#include "Player.h"

void Player::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	BoxCollider::Initialize();
	Collider::SetWorld(&world_);
	BoxCollider::SetSize({1.0f,2.0f,1.0f});
	SetcollitionAttribute(kCollitionAttributePlayer);
	SetcollisionMask(~kCollitionAttributePlayer);

}

void Player::Update()
{

#ifdef _DEBUG
	ImGui();
#endif
	//Gravity();
	BoxCollider::Update();
	world_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}

}

void Player::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::DragFloat3("Scale", &world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform_.translate.x);
	ImGui::End();
#endif
}

void Player::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute() == kCollitionAttributeFloor) {
#ifdef _DEBUG
		ImGui::Begin("PlayerAndPlane");
		
		ImGui::End();
#endif
		if (collider->GetCenter().y > world_.transform_.translate.y) {
			world_.transform_.translate.y = collider->GetCenter().y;
			world_.UpdateMatrix();
		}
	}
	return;
}

void Player::Gravity()
{
	world_.transform_.translate.y -= kGravity;
}
