#include "Enemy.h"

void Enemy::Initialize(std::vector<Model*> models)
{
	models_ = models;

	searchPoint = std::make_unique<SearchPoint>();
	searchPoint->Initalize(&world_,{10.0f,0.0f,10.0f});

	world_.Initialize();

	BoxCollider::Initialize();
	Collider::SetWorld(&world_);
	BoxCollider::SetSize({ 2.0f,2.0f,2.0f });
	SetcollitionAttribute(kCollitionAttributeEnemy);
	SetcollisionMask(~kCollitionAttributeEnemy && ~kCollitionAttributeSearchPoint);

}

void Enemy::Update()
{
	tlanslatePre = world_.transform_.translate;

	searchPoint->Update();

	if (searchPoint->GetIsSearchOn()) {
		world_.transform_.translate.y = 2.0f;
		world_.UpdateMatrix();
	}
	else {
		world_.transform_.translate.y = 0.0f;
		world_.UpdateMatrix();
	}
	BoxCollider::Update();
}

void Enemy::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}
}

void Enemy::ImGui()
{

}

void Enemy::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute() == kCollitionAttributePlayer) {
#ifdef _DEBUG
		ImGui::Begin("EnemyHitPlayer");

		ImGui::End();
#endif
	}	
	if (collider->GetcollitionAttribute() == kCollitionAttributeWeapon) {
#ifdef _DEBUG
		ImGui::Begin("EnemyHitWeapon");

		ImGui::End();
#endif
	}
}
