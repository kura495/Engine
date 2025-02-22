#include "BeamSword.h"
void BeamSword::Init()
{
	world_.Init();
	world_.transform.translate.y = 1.0f;
	world_.Update();
	collider_.Init(&world_);
	collider_.SetSize(Csize);
	collider_.SetOffset(Coffset);
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
void BeamSword::SetModelSize(Vector3 size)
{
	collider_.SetSize({ Csize.x,Csize.y,Csize.z * size.z });
	collider_.SetOffset({ Coffset.x,Coffset.y,Coffset.z * size.z });
	world_.transform.scale = size;
}
void BeamSword::OnCollision(ICollider& Collider)
{
	Collider;
}
