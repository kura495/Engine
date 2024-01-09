#include "Weapon.h"

void Weapon::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	world_.transform_.translate.x = 0.7f;
	world_.transform_.translate.z = 1.0f;

	Vector3 cross = Normalize(Cross({ 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f }));
	firstPos = MakeRotateAxisAngleQuaternion(cross, std::acos(-0.8f));
	world_.transform_.quaternion = firstPos;

	world_.UpdateMatrix();

	BoxCollider::Initialize();
	Collider::SetWorld(&world_);
	BoxCollider::SetSize({ 0.0f,0.0f,0.0f });
	SetcollitionAttribute(kCollitionAttributeWeapon);
	BoxCollider::SetcollisionMask(~kCollitionAttributePlayer && ~kCollitionAttributeWeapon);


}

void Weapon::Update()
{
	world_.UpdateMatrix();
	BoxCollider::Update();
}

void Weapon::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}
}

void Weapon::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Weapon");
	ImGui::DragFloat3("Scale", &world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform_.translate.x);
	if (ImGui::Button("Reset")) {
		world_.transform_.translate = { 0.0f,1.0f,0.0f };
	}
	ImGui::End();
#endif
}

void Weapon::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute()) {
		return;
	}
}

void Weapon::SetParent(const WorldTransform& parent)
{
	world_.parent_ = &parent;
}

void Weapon::AttackInit()
{
	Vector3 cross = Normalize(Cross({ 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f }));
	attackFirstQua = MakeRotateAxisAngleQuaternion(cross, std::acos(-1.0f));

	attackEndQua = MakeRotateAxisAngleQuaternion(cross, std::acos(0.0f));

	world_.transform_.quaternion = attackFirstQua;

	AttackQuaParam_t = 0.0f;
}

void Weapon::AttackUpdate()
{
	AttackQuaParam_t += 0.01f;

	world_.transform_.quaternion = Slerp(attackFirstQua, attackEndQua,AttackQuaParam_t);
}

