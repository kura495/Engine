#include "PlaneObject.h"

void PlaneObject::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();

	BoxCollider::Initialize();
	Collider::SetWorld(&world_);
	BoxCollider::SetSize({ 5.0f,0.0f,5.0f });
	SetcollitionAttribute(kCollitionAttributeFloor);
	SetcollisionMask(~kCollitionAttributeFloor);
}

void PlaneObject::Update()
{
	world_.UpdateMatrix();

	BoxCollider::Update();
}

void PlaneObject::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}
}

void PlaneObject::ImGui()
{
	ImGui::Begin("PlaneObject");
	ImGui::Text("Objectnumber %d", objectNumber_);
	ImGui::DragFloat3("Scale", &world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform_.translate.x);
	ImGui::End();
	std::string Number = std::to_string(GetNumber());

	std::string Name = "Plane" + Number;
	GlobalVariables::GetInstance()->UpdateTransformQuaItem("Editer", Name, world_.transform_);
}

void PlaneObject::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute()) {
		return;
	}
}
