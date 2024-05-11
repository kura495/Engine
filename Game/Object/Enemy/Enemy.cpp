#include "Enemy.h"
#include "Game/Object/Player/Player.h"

void Enemy::Initialize(std::vector<Model*> models)
{
	models_ = models;
	models[0]->GetModelData().material.textureFilePath;
	world_.Initialize();
	//world_.transform_.translate.y += 1.0f;
	animationSystem = new Animation();
	animationSystem->Init();

	animation = Animation::LoadAnimationFile("resources/human", "walk.gltf");
	skeleton = animationSystem->CreateSkeleton(models_[0]->GetModelData().rootNode);
	skinCluster = animationSystem->CreateSkinCluster(skeleton, models_[0]->GetModelData());

	//world_.SetTransform(models_[0]);
	animationSystem->ApplyAnimation(skeleton, animation, animationTime_);

	animationSystem->SkeletonUpdate(skeleton);
	animationSystem->SkinClusterUpdate(skinCluster, skeleton);
}

void Enemy::Update()
{

	if (ChackOnAttack()) {
		//範囲内なら何もしない
		return;
	}
	else {
		//範囲外なら歩く
		ChasePlayer();

		animationTime_ += 1.0f / 60.0f;

		animationTime_ = std::fmod(animationTime_, animation.duration);
	
		animationSystem->ApplyAnimation(skeleton, animation, animationTime_);

		animationSystem->SkeletonUpdate(skeleton);
		animationSystem->SkinClusterUpdate(skinCluster,skeleton);
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
