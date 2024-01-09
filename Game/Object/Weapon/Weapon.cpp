#include "Weapon.h"

void Weapon::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	world_.transform_.translate.x = 0.5f;
	world_.UpdateMatrix();

	BoxCollider::Initialize();
	BoxCollider::SetcollisionMask(~kCollitionAttributeWeapon && ~kCollitionAttributeEnemy);
	BoxCollider::SetSize({ 1.0f,1.0f,1.0f });
}

void Weapon::Update()
{
	world_.UpdateMatrix();
	BoxCollider::Update();
}

void Weapon::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}
}

void Weapon::ImGui()
{
}

void Weapon::OnCollision(const Collider* collider)
{
}

void Weapon::SetParent(const WorldTransform& parent)
{
	world_.parent_ = &parent;
}