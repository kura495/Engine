#include "PlaneObject.h"

void PlaneObject::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();

	BoxCollider::Init(&world_);
	BoxCollider::SetSize({ 5.0f,0.0f,5.0f });
	SetcollitionAttribute(ColliderTag::Floor);
	SetcollisionMask(~ColliderTag::Floor);
	BoxCollider::OnCollision = [this](ICollider* collider) { OnCollision(collider); };
}

void PlaneObject::Update()
{
	world_.Update();

	BoxCollider::SetSize({world_.transform.scale.x * 5.0f,0.0f,world_.transform.scale.z * 5.0f });
}

void PlaneObject::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}
}

void PlaneObject::ImGui()
{
	ImGui::Begin("PlaneObject");
	ImGui::Text("Objectnumber %d", objectNumber_);
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x);
	ImGui::End();
	std::string Number = std::to_string(GetNumber());

	std::string Name = "Plane" + Number;
	GlobalVariables::GetInstance()->UpdateTransformQuaItem("Editer", Name, world_.transform);
}

void PlaneObject::OnCollision(const ICollider* collider)
{
	if (collider->GetcollitionAttribute()) {
		return;
	}
}
