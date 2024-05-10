#include "Enemy.h"
#include "Game/Object/Player/Player.h"

void Enemy::Initialize(std::vector<Model*> models)
{
	models_ = models;
	models[0]->GetModelData().material.textureFilePath;
	world_.Initialize();
	//world_.transform_.translate.y += 1.0f;

	animation = Animation::LoadAnimationFile("resources/human", "sneakwalk.gltf");
	skeleton = models_[0]->CreateSkeleton(models_[0]->GetModelData().rootNode);
	skinCluster = models_[0]->CreateSkinCluster(skeleton, models_[0]->GetModelData());

	//world_.SetTransform(models_[0]);
	models_[0]->ApplyAnimation(skeleton, animation, animationTime_);

	models_[0]->SkeletonUpdate(skeleton);
	models_[0]->SkinClusterUpdate(skinCluster, skeleton);
}

void Enemy::Update()
{

	if (ChackOnAttack()) {
		//範囲内なら何もしない
		return;
	}
	else {
		//範囲外なら歩く
		//ChasePlayer();

		animationTime_ += 1.0f / 60.0f;

		animationTime_ = std::fmod(animationTime_, animation.duration);
	
		models_[0]->ApplyAnimation(skeleton, animation, animationTime_);

		models_[0]->SkeletonUpdate(skeleton);
		models_[0]->SkinClusterUpdate(skinCluster,skeleton);
	}


	world_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->SkinDraw(world_, viewProj, skinCluster);
	}
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
