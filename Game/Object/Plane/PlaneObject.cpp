#include "PlaneObject.h"

void PlaneObject::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	world_.Update();
	collider.Init(&world_);
	collider.SetSize({ 5.0f,0.0f,5.0f });
	collider.SetcollitionAttribute(ColliderTag::Floor);
	collider.SetcollisionMask(~ColliderTag::Floor);
	collider.OnCollision = [this](ICollider& collider) { OnCollision(collider); };
}

void PlaneObject::Update()
{
	world_.Update();
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

void PlaneObject::OnCollision(const ICollider& Collider)
{
	if (Collider.GetcollitionAttribute() == ColliderTag::EnemyAttack) {
		isCrash = true;
		ImGui::Begin("Plane");
		ImGui::Text("Hit");
		ImGui::End();
		return;
	}
}

void PlaneObject::CrashEffect()
{

}
