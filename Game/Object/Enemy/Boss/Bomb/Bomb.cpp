#include "Bomb.h"

void Bomb::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	ColliderInit();
}

void Bomb::Update()
{

	ImGui();

	world_.Update();
	//一定ラインを超えたら止める
	distance = Vector3::Distance(stertPos, world_.transform.translate);
	if (distance >= 80.0f) {
		IsOverline = true;
	}
	else {
		IsOverline = false;
	}

	if (IsOverline) {
		return;
	}

	//移動の制限(下限と上限を一行で書いている)
	world_.transform.translate.y = (std::max)(world_.transform.translate.y,0.5f);

	if (isThrowFlag) {
		world_.transform.translate += forTargetVector;
	}

	easeT = (std::min)(easeT + addEaseT, 1.0f);

}

void Bomb::Draw()
{
	models_[0]->RendererDraw(world_);
}

void Bomb::ColliderInit()
{
	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
	collider.OnCollision = [this](ICollider& colliderA) { OnCollision(colliderA); };
	collider.SetcollitionAttribute(ColliderTag::EnemyBomb);
	collider.IsUsing = true;
}

void Bomb::OnCollision(const ICollider& colliderA)
{
	if (colliderA.GetcollitionAttribute() == ColliderTag::Weapon) {
		if (isHit) {
			forTargetVector *= -1.2f;
			easeT = 0.0f;
			isHit = false;
		}
	}
	if (colliderA.GetcollitionAttribute() == ColliderTag::Enemy){
		if (isHit == false) {
			forTargetVector *= -1.2f;
			easeT = 0.0f;
			isHit = true;
		}
	}
	if (colliderA.GetcollitionAttribute() == ColliderTag::Player) {
		collider.IsUsing = false;
	}
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
