#include "BossBullet.h"
#include "Game/Object/Player/Player.h"

void BossBullet::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	InitCollider();

	world_.transform.translate.y = 10.0f;

	targetPos = player_->GetWorld().transform.translate - world_.transform.translate;

	AddPos = targetPos.Normalize();
}

void BossBullet::Update()
{
	world_.transform.translate += AddPos;

	world_.UpdateMatrix();
}

void BossBullet::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}
}

void BossBullet::InitCollider()
{
	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
	collider.OnCollision = [this](ICollider* colliderB) { OnCollision(colliderB); };
	collider.SetcollitionAttribute(ColliderTag::BossBullet);
	collider.SetcollisionMask(~ColliderTag::BossBullet & ~ColliderTag::Enemy);
	//攻撃の当たり判定用座標
}

void BossBullet::OnCollision(ICollider* colliderB)
{
	if (colliderB->GetcollitionAttribute() == ColliderTag::Player) {
		collider.IsUsing = false;
		IsDead_ = true;
	}
}
