#include "Bomb.h"

void Bomb::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	ColliderInit();
	world_.transform.translate.z = -11.0f;
}

void Bomb::Update()
{

	ImGui();

	if (IsOverline) {
		return;
	}

	//移動の制限(下限と上限を一行で書いている)
	world_.transform.translate.y = (std::max)(world_.transform.translate.y,0.5f);

	if (isThrowFlag) {
		world_.transform.translate += forTargetVector;
	}
	world_.Update();
	distance = Vector3::Distance({ 0.0f,0.0f,-10.0f }, world_.transform.translate);
	if (distance >= 10.0f) {
		IsOverline = true;
	}
}

void Bomb::Draw()
{
	models_[0]->RendererDraw(world_);
}

void Bomb::ColliderInit()
{
	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
	collider.OnCollision = [this](ICollider* colliderA) { OnCollision(colliderA); };
	collider.SetcollitionAttribute(ColliderTag::Enemy);
	collider.SetcollisionMask(~ColliderTag::EnemyBomb);
	collider.IsUsing = true;
}

void Bomb::OnCollision(const ICollider* colliderA)
{
	colliderA;
}

void Bomb::ImGui()
{
	ImGui::Begin("Bomb");
	ImGui::InputFloat("z",&world_.transform.translate.z);
	ImGui::InputFloat("length",&distance);
	if (IsOverline) {
		ImGui::Text("IsOverline" );
	}

	ImGui::End();
}
