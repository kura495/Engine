#include "Enemy.h"

void Enemy::Initialize(std::vector<Model*> models)
{
	models_ = models;

	searchPoint = std::make_unique<SearchPoint>();
	searchPoint->Initalize(&world_,{10.0f,0.0f,10.0f});

	world_.Initialize();

	BoxCollider::Initialize();
	Collider::SetWorld(&world_);
	BoxCollider::SetSize({ 0.5f,1.0f,0.5f });
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
		Vector3 colliderPos = collider->GetCenter();

#pragma region
		if (tlanslatePre.x > colliderPos.x + collider->GetSize().x) {
			//左から右
			if (world_.transform_.translate.x - Collider::GetSize().x < colliderPos.x + collider->GetSize().x) {
				world_.transform_.translate.x = colliderPos.x + collider->GetSize().x + Collider::GetSize().x;
			}
		}
		if (tlanslatePre.x < colliderPos.x - collider->GetSize().x) {
			//右から左
			if (world_.transform_.translate.x + Collider::GetSize().x > colliderPos.x - collider->GetSize().x) {
				world_.transform_.translate.x = colliderPos.x - collider->GetSize().x - Collider::GetSize().x;
			}
		}
		if (tlanslatePre.z < colliderPos.z - collider->GetSize().z) {
			//手前から奥
			if (world_.transform_.translate.z + Collider::GetSize().z > colliderPos.z - collider->GetSize().z) {
				world_.transform_.translate.z = colliderPos.z - collider->GetSize().z - Collider::GetSize().z;
			}
		}
		if (tlanslatePre.z > colliderPos.z + collider->GetSize().z) {
			//奥から手前
			if (world_.transform_.translate.z - Collider::GetSize().z < colliderPos.z + collider->GetSize().z) {
				world_.transform_.translate.z = colliderPos.z + collider->GetSize().z + Collider::GetSize().z;
			}
		}
#pragma endregion 移動制御	

		world_.UpdateMatrix();
	}
}
