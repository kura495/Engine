#include "BoxObject.h"

void BoxObject::Init(std::vector<Model*> models)
{

	models_ = models;
	world_.Initialize();

	BoxCollider::Init(&world_);
	BoxCollider::SetSize({ 1.0f,1.0f,1.0f });
	BoxCollider::OnCollision = [this](ICollider& collider) { OnCollision(collider); };
	//SetcollitionAttribute(Collider::Tag::Box);
	//SetcollisionMask(~Collider::Tag::Box);
}

void BoxObject::Update()
{
	world_.Update();

	std::string Number = std::to_string(GetNumber());
	std::string Name = "Box" + Number;
	GlobalVariables::GetInstance()->UpdateTransformQuaItem("Editer", Name, world_.transform);
}

void BoxObject::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}
}

void BoxObject::ImGui()
{
	ImGui::Begin("Box");
	ImGui::Text("Objectnumber %d", objectNumber_);
	ImGui::DragFloat3("Scale",&world_.transform.scale.x);
	ImGui::DragFloat4("Rotate",&world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate",&world_.transform.translate.x);
	world_.Update();
	ImGui::End();


}

void BoxObject::OnCollision(const ICollider& collider)
{
	if (collider.GetcollitionAttribute()) {
		return;
	}
}

