#include "Bomb.h"

void Bomb::Init(std::vector<Model*> models)
{
	world_.Initialize();
}

void Bomb::Update()
{
	//world_.transform.translate.x = std::min(boxMin.x, boxMax.x);
	//ˆÚ“®‚Ì§ŒÀ(‰ºŒÀ‚ÆãŒÀ‚ðˆês‚Å‘‚¢‚Ä‚¢‚é)
	world_.transform.translate.x = (std::max)((std::min)(world_.transform.translate.x, boxMax.x), boxMin.x);

	if (isThrowFlag) {
		world_.transform.translate += forTargetVector;
	}

}

void Bomb::Draw()
{
	models_[0]->RendererDraw(world_);
}

void Bomb::ColliderInit()
{
	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
	collider.OnCollision = [this](ICollider* colliderA) { OnCollision(colliderA); };
	collider.SetcollitionAttribute(ColliderTag::Enemy);
	collider.SetcollisionMask(~ColliderTag::EnemyBomb);
	collider.IsUsing = true;
}

void Bomb::OnCollision(const ICollider* colliderA)
{
	colliderA;
}
