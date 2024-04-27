#include "Enemy.h"
#include "Game/Object/Player/Player.h"

void Enemy::Initialize(std::vector<Model*> models)
{
	models_ = models;
	models[0]->GetModelData().material.textureFilePath;
	world_.Initialize();
	world_.transform_.translate.y += 1.0f;
	animation = new Animation();
	*animation = Animation::LoadAnimationFile("resources/Monster", "Monster.gltf");
	world_.SetTransform(models_[0]);

}

void Enemy::Update()
{
	animeInterval_ += 1.0f;

	if (isAttackFlag == false) {
		if (ChackOnAttack() && animeInterval_ > kAnimeInterval) {
			isAttackFlag = true;
		}
		else {
			ChasePlayer();
		}

	}
	// アニメーション全体の再生が終わったら

	if (isAttackFlag) {
		animationTime_ += 1.0f / 60.0f;
		if (animationTime_ > animation->duration) {
			isAttackFlag = false;
			animeInterval_ = 0.0f;
		}
		animationTime_ = std::fmod(animationTime_, animation->duration);
	
		NodeAnimation& rootNodeAnimation = animation->nodeAnimations[models_[0]->GetModelData().rootNode.name];
		Vector3 translate = Animation::CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime_);
		Quaternion rotation = Animation::CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime_);
		Vector3 scale = Animation::CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime_);
		world_.transform_.translate += translate;
		world_.transform_.quaternion += rotation;
		world_.transform_.scale = scale;
	}
	world_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}
}

void Enemy::ImGui()
{

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
