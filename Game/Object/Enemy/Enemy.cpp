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
	
	attackWorld_.Initialize();
	attackWorld_.SetParent(&world_);
	attackWorld_.transform.translate = {0.0f,0.0f,1.7f};
	attackCollider.Init(&attackWorld_);
	attackCollider.SetSize({1.0f,1.0f,0.7f});
	attackCollider.OnCollision = [this](ICollider* collider) { AttackOnCollision(collider); };
	attackCollider.SetcollitionAttribute(ColliderTag::EnemyAttack);
	attackCollider.SetcollisionMask(~ColliderTag::Enemy && ~ColliderTag::EnemyAttack);
	attackCollider.IsUsing = false;
#pragma region

	world_.transform.translate.y += 1.0f;
	animation = Animation::LoadAnimationFile("resources/Monster", "Monster.gltf");

	world_.SetTransform(models_[0]);
	animation->Init();
	animation->AnimeInit(*models_[0],false);

#pragma endregion Animation

}

void Enemy::Update()
{
	ImGui();
	if (ChackOnAttack() && isAttackFlag == false) {
		isAttackFlag = true;
		attackColliderFlag = true;
		//範囲内なら攻撃
	}

	if (isAttackFlag) {
		PlayAnime();
	}
	else {
		//範囲外なら歩く
		ChasePlayer();
	}

	world_.UpdateMatrix();
}

void Enemy::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}
}

void Enemy::PlayAnime()
{
	if (isAttackFlag) {
		animationTime_ += 2.0f / 60.0f;
		if (animationTime_ > animation->duration) {
			isAttackFlag = false;
			animeInterval_ = 0.0f;
			attackCollider.IsUsing = false;
		}
		animationTime_ = std::fmod(animationTime_, animation->duration);

		if (attackColliderFlag) {
			if (animationTime_ >= 1.7f) {
				attackCollider.IsUsing = true;
				attackColliderFlag = false;
			}
		}


		NodeAnimation& rootNodeAnimation = animation->nodeAnimations[models_[0]->GetModelData().rootNode.name];
		Vector3 translate = Animation::CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime_);
		Quaternion rotation = Animation::CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime_);
		Vector3 scale = Animation::CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime_);
		world_.transform.translate += translate;
		world_.transform.quaternion = rotation;
		world_.transform.scale = scale;
	}
	world_.UpdateMatrix();
}

void Enemy::ImGui()
{
#ifdef USE_IMGUI
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x);
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
	if (ICollider->GetcollitionAttribute() == ColliderTag::Weapon) {
		HP_ -= 1;
	}
	if (ICollider->GetcollitionAttribute() == ColliderTag::Player) {
	}
}

void Enemy::AttackOnCollision(const ICollider* ICollider)
{
	if (ICollider->GetcollitionAttribute() == ColliderTag::Player) {
		attackCollider.IsUsing = false;
	}
}

bool Enemy::ChackOnAttack()
{
	Vector3 PtoEdistance = player_->GetWorldTransform().transform.translate - world_.transform.translate;
	float Distance = PtoEdistance.Length();
	if (Distance < AttackRange) {
		return true;
	}
	return false;
}

void Enemy::ChasePlayer()
{
	Vector3 PtoEdistance = player_->GetWorldTransform().transform.translate - world_.transform.translate;
	float Distance = PtoEdistance.Length();
	if (Distance < AttackRange) {
		return;
	}
	world_.transform.translate += PtoEdistance / 60.0f;
}
