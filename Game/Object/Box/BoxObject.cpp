#include "BoxObject.h"

void BoxObject::Initalize(std::vector<Model*> models)
{


	models_ = models;
	world_.Initialize();

	ICollider::SetWorld(&world_);
	BoxCollider::Initialize();
	//BoxCollider::SetSize({ 1.0f,1.0f,1.0f });
	SetcollitionAttribute(kCollitionAttributeBox);
	SetcollisionMask(~kCollitionAttributeBox);
}

void BoxObject::Update()
{
	world_.UpdateMatrix();
	////Scene読み込みとの兼ね合いでコメントアウト
	//BoxCollider::SetSize({world_.transform_.scale.x,world_.transform_.scale.y,world_.transform_.scale.z});

	std::string Number = std::to_string(GetNumber());
	std::string Name = "Box" + Number;
	GlobalVariables::GetInstance()->UpdateTransformQuaItem("Editer", Name, world_.transform_);
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
	ImGui::DragFloat3("Scale",&world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate",&world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate",&world_.transform_.translate.x);
	world_.UpdateMatrix();
	ImGui::End();


}

void BoxObject::OnCollision(const ICollider* collider)
{
	if (collider->GetcollitionAttribute()) {
		return;
	}
}

