#include "Sword.h"

void Sword::Init()
{
	world_.Init();
	collider_.Init(&world_);
	collider_.SetSize({ 1.0f,1.0f,1.0f });
	collider_.SetcollitionAttribute(Collider::Tag::EnemyAttack);
	collider_.SetcollisionMask(~Collider::Tag::Enemy);
	collider_.OnCollision = [this](ICollider& Collider) { OnCollision(Collider); };

	model_ = std::make_unique<Model>();
	model_->Initialize("project/resources/Enemy/Sword", "Sword.obj");
}
void Sword::Update()
{

}
void Sword::Draw()
{
	model_->RendererDraw(world_);
}
void Sword::OnCollision(ICollider& Collider)
{
	Collider;
}
