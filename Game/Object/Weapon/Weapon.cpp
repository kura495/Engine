#include "Weapon.h"

void Weapon::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	world_.transform_.translate.x = 0.7f;
	world_.transform_.translate.z = 1.0f;

	Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f }));
	firstPos = MakeRotateAxisAngleQuaternion(cross, std::acos(-0.8f));
	world_.transform_.quaternion = firstPos;

	world_.UpdateMatrix();

	OBBoxCollider::Init(&world_);
	OBBoxCollider::SetSize({ 0.0f,0.0f,0.0f });
	SetcollitionAttribute(kCollitionAttributeWeapon);
	OBBoxCollider::SetcollisionMask(~kCollitionAttributePlayer && ~kCollitionAttributeWeapon);


}

void Weapon::Update()
{
	world_.UpdateMatrix();
}

void Weapon::Draw()
{
	models_[1]->RendererDraw(world_);
}

void Weapon::ImGui()
{
#ifdef USE_IMGUI
	ImGui::Begin("Weapon");
	ImGui::DragFloat3("Scale", &world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform_.translate.x);
	Vector3 Cen = OBBoxCollider::GetSize();
	ImGui::DragFloat3("Size", &Cen.x);
	

	ImGui::End();
#endif
}

void Weapon::OnCollision(const ICollider* collider)
{
	if (collider->GetcollitionAttribute() == kCollitionAttributeEnemy) {
#ifdef USE_IMGUI
		ImGui::Begin("A");

		ImGui::End();
#endif
	}

	if (collider->GetcollitionAttribute() == kCollitionAttributeBox) {

	}
}

void Weapon::SetParent(const WorldTransform& parent)
{
	world_.parent_ = &parent;
}

void Weapon::RootInit()
{
	world_.transform_.translate.x = 0.7f;
	world_.transform_.translate.z = 1.0f;
	world_.transform_.quaternion = firstPos;
	world_.UpdateMatrix();

	OBBoxCollider::SetSize({ 0.0f,0.0f,0.0f });
}

void Weapon::RootUpdate()
{
	IsAttackOver = false;
}

void Weapon::AttackInit()
{
	Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f }));
	attackFirstQua = MakeRotateAxisAngleQuaternion(cross, std::acos(-1.0f));

	attackEndQua = MakeRotateAxisAngleQuaternion(cross, std::acos(0.0f));
	OBBoxCollider::SetSize({ 0.5f,2.0f,0.5f });

	world_.transform_.quaternion = attackFirstQua;
	world_.transform_.translate.x = 0.5f;
	world_.transform_.translate.y = 0.5f;
	world_.transform_.translate.z = 2.0f;
	world_.UpdateMatrix();

	AttackQuaParam_t = 0.0f;
	IsAttackOver = false;
}

void Weapon::AttackUpdate()
{
	AttackQuaParam_t += 0.05f;

	world_.transform_.quaternion = Quaternion::Slerp(attackFirstQua, attackEndQua,AttackQuaParam_t);

	if (AttackQuaParam_t >=1.0f) {
		IsAttackOver = true;

	}
}


