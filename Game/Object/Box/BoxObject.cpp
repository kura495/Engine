#include "BoxObject.h"

void BoxObject::Init(std::vector<Model*> models)
{


	models_ = models;
	world_.Initialize();

	BoxCollider::Initialize(&world_);
	//BoxCollider::SetSize({ 1.0f,1.0f,1.0f });
	BoxCollider::OnCollision = [this](ICollider* collider) { OnCollision(collider); };
	SetcollitionAttribute(ColliderTag::Box);
	SetcollisionMask(~ColliderTag::Box);
}

void BoxObject::Update()
{
	world_.UpdateMatrix();
	////Scene読み込みとの兼ね合いでコメントアウト
	//BoxCollider::SetSize({world_.transform.scale.x,world_.transform.scale.y,world_.transform.scale.z});

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
	world_.UpdateMatrix();
	ImGui::End();


}

void BoxObject::OnCollision(const ICollider* collider)
{
	if (collider->GetcollitionAttribute()) {
		return;
	}
}

