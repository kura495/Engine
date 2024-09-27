#include "Spider.h"

void Spider::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();

	//Collider
	InitCollider();

	world_.transform.translate.y = -1.0f;
#pragma region

	animation = Animation::LoadAnimationFile("resources/Monster", "Monster.gltf");

	world_.SetTransform(models_[0]);
	animation->Init();
	animation->AnimeInit(*models_[0], false);

#pragma endregion Animation
}

void Spider::Update()
{
	if (HP_ <= 0) {
		IsAlive = false;
	}
	//出現の時のアニメーション
	if (spawnFlame_ > kSpawnFlame_) {
		world_.transform.translate.y = 1.0f;
		isSpawn = false;
	}
	else if (isSpawn) {
		if (spawnFlame_ <= 40) {
			world_.transform.translate.y += 0.1f;
		}
		else {
			world_.transform.translate.y -= 0.1f;
		}
		spawnFlame_++;

		world_.UpdateMatrix();
		return;
	}

	ImGui();
	//TODO:BehaviorTreeに変更する！
	//範囲内なら攻撃
	if (ChackOnAttack() && isAttackFlag == false) {
		isAttackFlag = true;
		attackColliderFlag = true;
		//攻撃の初期化
		AttackInit();
	}

	if (isAttackFlag) {

		PlayAttackAnime();
	}
	else {
		//範囲外なら歩く
		ChasePlayer();
		LookPlayer();
	}

	DamageEffect();

	world_.UpdateMatrix();
}

void Spider::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}
}

void Spider::InitCollider()
{

	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
	collider.OnCollision = [this](ICollider* colliderB) { OnCollision(colliderB); };
	collider.SetcollitionAttribute(ColliderTag::Enemy);
	collider.SetcollisionMask(~ColliderTag::Enemy);
	//攻撃の当たり判定用座標
	attackWorld_.Initialize();
	attackWorld_.SetParent(&world_);
	attackWorld_.transform.translate = { 0.0f,0.0f,1.7f };
	attackCollider.Init(&attackWorld_);
	attackCollider.SetSize({ 1.0f,1.0f,0.7f });
	attackCollider.OnCollision = [this](ICollider* colliderB) { AttackOnCollision(colliderB); };
	attackCollider.SetcollitionAttribute(ColliderTag::EnemyAttack);
	attackCollider.SetcollisionMask(~ColliderTag::Enemy && ~ColliderTag::EnemyAttack);
	attackCollider.IsUsing = false;

}
