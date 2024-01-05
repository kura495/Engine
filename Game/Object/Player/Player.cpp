#include "Player.h"

void Player::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	BoxCollider::Initialize();
	BoxCollider::SetWorld(&world_);
	BoxCollider::SetSize({1.0f,2.0f,1.0f});
	SetcollitionAttribute(kCollitionAttributePlayer);
	SetcollisionMask(~kCollitionAttributePlayer);

}

void Player::Update()
{

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
}

void Player::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute()) {
		return;
	}
}
