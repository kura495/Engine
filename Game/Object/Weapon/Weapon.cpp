#include "Weapon.h"

void Weapon::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	world_.transform.translate.x = 0.7f;
	world_.transform.translate.z = 1.0f;

	Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f }));
	firstPos = Quaternion::MakeRotateAxisAngleQuaternion(cross, std::acos(-0.8f));
	world_.transform.quaternion = firstPos;

	world_.UpdateMatrix();

	collider.Init(&world_);
	collider.SetSize({ 0.5f,2.0f,0.5f });
	collider.OnCollision = [this](ICollider* collider) { return OnCollision(collider);};
	collider.SetcollitionAttribute(ColliderTag::Weapon);
	collider.SetcollisionMask(~ColliderTag::Player & ~ColliderTag::Weapon);
	collider.IsUsing = false;

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
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x);
	Vector3 Cen = collider.GetSize();
	ImGui::DragFloat3("Size", &Cen.x);
	if (ImGui::Button("CollisionOn")) {
		collider.IsUsing = true;
	}
	if (ImGui::Button("CollisionOff")) {
		collider.IsUsing = false;
	}

	ImGui::End();
#endif
}

void Weapon::OnCollision(const ICollider* Icollider)
{		
	if (Icollider->GetcollitionAttribute() == ColliderTag::Enemy) {
		collider.IsUsing = false;
	}
}

void Weapon::SetParent(const WorldTransform& parent)
{
	world_.SetParent(&parent);
}

void Weapon::RootInit()
{
	world_.transform.translate.x = 0.7f;
	world_.transform.translate.z = 1.0f;
	world_.transform.quaternion = firstPos;
	world_.UpdateMatrix();
	collider.IsUsing = false;
	collider.SetSize({ 0.5f,2.0f,0.5f });
}

void Weapon::RootUpdate()
{
	IsAttackOver = false;
}

void Weapon::AttackInit()
{
	Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f }));
	attackFirstQua = Quaternion::MakeRotateAxisAngleQuaternion(cross, std::acos(-1.0f));

	attackEndQua = Quaternion::MakeRotateAxisAngleQuaternion(cross, std::acos(0.0f));
	collider.SetSize({ 0.5f,2.0f,0.5f });
	collider.IsUsing = true;

	world_.transform.quaternion = attackFirstQua;
	world_.transform.translate.x = 0.5f;
	world_.transform.translate.y = 0.5f;
	world_.transform.translate.z = 2.0f;
	world_.UpdateMatrix();

	AttackQuaParam_t = 0.0f;
	IsAttackOver = false;
}

void Weapon::AttackUpdate()
{
	AttackQuaParam_t += 0.05f;

	world_.transform.quaternion = Quaternion::Slerp(attackFirstQua, attackEndQua,AttackQuaParam_t);

	if (AttackQuaParam_t >=1.0f) {
		IsAttackOver = true;

	}
}


