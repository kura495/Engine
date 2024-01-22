#include "Goal.h"

void Goal::Initialize(const std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	world_.transform_.translate = { -22.0f,1.0f,16.0f };
	world_.UpdateMatrix();
	BoxCollider::Initialize();
	BoxCollider::SetSize({1.0f,1.0f,1.0f});
	BoxCollider::SetcollitionAttribute(kCollitionAttributeGoal);
	BoxCollider::SetcollisionMask(~kCollitionAttributeGoal);
	Collider::SetWorld(&world_);
}

void Goal::Update()
{
	world_.UpdateMatrix();
	BoxCollider::Update();
}

void Goal::Draw(const ViewProjection& viewProjection)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProjection);
	}
}

void Goal::ImGui()
{
}

void Goal::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute()) {
		return;
	}
}

