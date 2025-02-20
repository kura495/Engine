#include "BeamSword.h"
void BeamSword::Init()
{
	world_.Init();
	world_.transform.translate.y = 1.5f;
	world_.Update();
	collider_.Init(&world_);
	collider_.SetSize({ 1.0f,0.5f,5.0f });
	collider_.SetOffset({ 0.0f,0.0f,-4.0f });
	//collider_.SetcollitionAttribute(Collider::Tag::EnemyAttack);
	collider_.SetcollisionMask(~Collider::Tag::Enemy);
	collider_.OnCollision = [this](ICollider& Collider) { OnCollision(Collider); };

	model_ = std::make_unique<Model>();
	model_->Initialize("project/resources/Enemy/Sword", "Sword.obj");
}
void BeamSword::Update()
{
	world_.Update();

}
void BeamSword::Draw()
{
	model_->RendererDraw(world_);
}
void BeamSword::OnCollision(ICollider& Collider)
{
	Collider;
}
