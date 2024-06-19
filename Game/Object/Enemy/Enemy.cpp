#include "Enemy.h"
#include "Game/Object/Player/Player.h"

void Enemy::Initialize(std::vector<Model*> models)
{
	models_ = models;
	models[0]->GetModelData().material.textureFilePath;
	world_.Initialize();

	BoxCollider::Initialize();
	Collider::SetWorld(&world_);
	BoxCollider::SetSize({1.0f,1.0f,1.0f});

#pragma region
	animation = Animation::LoadAnimationFile("resources/human", "human.gltf");
	animation->Init();
	animation->AnimeInit(*models_[0]);

#pragma endregion Animation

}

void Enemy::Update()
{

	if (ChackOnAttack()) {
		//範囲内なら何もしない
		
	}
	else {
		//範囲外なら歩く
		ChasePlayer();

		animation->PlayAnimation();
	}
	BoxCollider::Update();
	world_.UpdateMatrix();
}

void Enemy::Draw()
{
#ifdef USE_IMGUI
	ImGui::Begin("EnemyDraw");
	ImGui::Checkbox("ModelDraw", &chackBoxflag);
	ImGui::End();

#endif
	if (chackBoxflag) {
		for (Model* model : models_) {
			model->RendererSkinDraw(world_,animation->GetSkinCluster());
		}
	}
	animation->DebugDraw(world_);
}

void Enemy::OnCollision(const Collider* collider)
{
	collider;
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
