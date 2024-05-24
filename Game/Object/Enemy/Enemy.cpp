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

	animation = Animation::LoadAnimationFile("resources/human", "human.gltf");
	skeleton = animationSystem->CreateSkeleton(models_[0]->GetModelData().rootNode);
	skinCluster = animationSystem->CreateSkinCluster(skeleton, models_[0]->GetModelData());

	animationTime_ += 1.0f / 60.0f;

	animationSystem->ApplyAnimation(skeleton, animation, animationTime_);

	animationSystem->SkeletonUpdate(skeleton);
	animationSystem->SkinClusterUpdate(skinCluster, skeleton);
	
	animationSystem->CreateBoneLineVertices(skeleton, skeleton.root, point);
	
	Lineworld_.Initialize();
	line = new Line();
	line->Init();
	line->SetVertexData(point);
	line->CreateBuffer();
	UpdateLine();
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

		UpdateLine();
	}
	
	world_.UpdateMatrix();
}

void Enemy::Draw()
{
	ImGui::Begin("EnemyDraw");
	ImGui::Checkbox("ModelDraw", &chackBoxflag);
	ImGui::End();
	if (chackBoxflag) {
		for (Model* model : models_) {
			model->RendererSkinDraw(world_,skinCluster);
		}
	}

}

void Enemy::DabugDraw()
{

	line->RendererDraw(world_);
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

void Enemy::UpdateLine()
{
	point.clear();
	const Joint& parentJoint = skeleton.joints[skeleton.root];
	for (int32_t childIndex : parentJoint.children)
	{
		const Joint& childJoint = skeleton.joints[childIndex];
		point.push_back({ parentJoint.skeletonSpaceMatrix.m[3][0],parentJoint.skeletonSpaceMatrix.m[3][1],parentJoint.skeletonSpaceMatrix.m[3][2],1.0f });
		point.push_back({ childJoint.skeletonSpaceMatrix.m[3][0],childJoint.skeletonSpaceMatrix.m[3][1],childJoint.skeletonSpaceMatrix.m[3][2],1.0f });
		animationSystem->CreateBoneLineVertices(skeleton, childIndex, point);
	}

	line->UpdateVertexData(point);
}
