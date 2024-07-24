#include "Enemy.h"
#include "Game/Object/Player/Player.h"

void Enemy::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();

	collider.Init(&world_);
	collider.SetSize({1.0f,1.0f,1.0f});
	collider.OnCollision = [this](ICollider* collider) { OnCollision(collider); };
	collider.SetcollitionAttribute(ColliderTag::Enemy);
	collider.SetcollisionMask(~ColliderTag::Enemy);
#pragma region
	animation = Animation::LoadAnimationFile("resources/human", "human.gltf");
	animation->Init();
	animation->AnimeInit(*models_[0]);

#pragma endregion Animation

}

void Enemy::Update()
{
	ImGui();
	if (ChackOnAttack()) {
		//範囲内なら何もしない
		
	}
	else {
		//範囲外なら歩く
		//ChasePlayer();

		animation->PlayAnimation();
	}
	world_.UpdateMatrix();
}

void Enemy::Draw()
{
	for (Model* model : models_) {
		model->RendererSkinDraw(world_,animation->GetSkinCluster());
	}

	animation->DebugDraw(world_);
}

void Enemy::ImGui()
{
#ifdef USE_IMGUI
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Scale", &world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform_.translate.x);
	if (ImGui::Button("CollisionOn")) {
		collider.IsUsing = true;
	}
	if (ImGui::Button("CollisionOff")) {
		collider.IsUsing = false;
	}
	ImGui::Text("HP : %d", HP_);
	ImGui::End();
#endif
}
	
void Enemy::OnCollision(const ICollider* ICollider)
{
	ImGui::Begin("EnemyCollider");
	if (ICollider->GetcollitionAttribute() == ColliderTag::Weapon) {
		ImGui::Text("WeaponHit");
		HP_ = 0;
	}
	if (ICollider->GetcollitionAttribute() == ColliderTag::Player) {
		ImGui::Text("PlayerHit");
	}


	ImGui::End();
}

bool Enemy::ChackOnAttack()
{
	Vector3 PtoEdistance = player_->GetWorldTransform().transform_.translate - world_.transform_.translate;
	float Distance = PtoEdistance.Length();
	if (Distance < AttackRange) {
		return true;
	}
	return false;
}

void Enemy::ChasePlayer()
{
	Vector3 PtoEdistance = player_->GetWorldTransform().transform_.translate - world_.transform_.translate;
	float Distance = PtoEdistance.Length();
	if (Distance < AttackRange) {
		return;
	}
	world_.transform_.translate += PtoEdistance / 60.0f;
}
