#include "BoxObject.h"

void BoxObject::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();

	BoxCollider::Initialize();
	Collider::SetWorld(&world_);
	BoxCollider::SetSize({ 1.0f,1.0f,1.0f });
	SetcollitionAttribute(kCollitionAttributeBox);
	SetcollisionMask(~kCollitionAttributeBox);
}

void BoxObject::Update()
{
	world_.UpdateMatrix();
	BoxCollider::SetSize({
	world_.transform_.scale.x,world_.transform_.scale.y,world_.transform_.scale.z});
	BoxCollider::Update();
}

void BoxObject::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}
}

void BoxObject::ImGui()
{
	ImGui::Begin("Box");
	ImGui::Text("Objectnumber %d", objectNumber_);
	ImGui::DragFloat3("Scale",&world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate",&world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate",&world_.transform_.translate.x);
	ImGui::End();
	std::string Number = std::to_string(GetNumber());

	std::string Name = "Box" + Number;
	GlobalVariables::GetInstance()->UpdateTransformQuaItem("Editer", Name,world_.transform_);
}

void BoxObject::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute()) {
		return;
	}
}

