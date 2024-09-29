#include "BossSpider.h"
#include "Game/Object/Player/Player.h"

void BossSpider::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();

	//Collider
	InitCollider();

	world_.transform.translate.z = 10.0f;
	world_.transform.translate.y = 1.0f;
	world_.UpdateMatrix();

	HP_ = 10;
}

void BossSpider::Update()
{
	ImGui();

	if (HP_ <= 0) {
		IsAlive = false;
	}

	if (behaviorRequest_) {
		//ふるまいの変更
		behavior_ = behaviorRequest_.value();
		//各ふるまいごとに初期化
		switch (behavior_)
		{
		case BossSpiderBehavior::kRoot:
		default:
			RootInit();
			break;
		case BossSpiderBehavior::kAttack:
			AttackInit();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}
	//更新
	switch (behavior_)
	{
	case BossSpiderBehavior::kRoot:
	default:
		RootUpdate();
		break;
	case BossSpiderBehavior::kAttack:
		AttackUpdate();
		break;
	}
#pragma endregion BehaviorTree

	for (BossBullet* bossBullet : bossBullet_) {
		bossBullet->Update();
	}

	world_.UpdateMatrix();
}

void BossSpider::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}

	for (BossBullet* bossBullet : bossBullet_) {
		bossBullet->Draw();
	}
}

void BossSpider::InitCollider()
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
void BossSpider::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("BossSpider");
	if (ImGui::Button("Attack")) {
		AttackInit();
	}
	ImGui::End();
#endif
}
#pragma region

void BossSpider::RootInit()
{

}

void BossSpider::RootUpdate()
{
	LookPlayer();
}

void BossSpider::AttackInit()
{
	std::vector<Model*> enemyModel_;
	enemyModel_.push_back(Model::CreateModelFromObj("resources/Cube", "Cube.obj"));
	bossBullet_.push_back(new BossBullet(player_));
	for (BossBullet* bossBullet : bossBullet_) {
		bossBullet->Init(enemyModel_);
	}
}

void BossSpider::AttackUpdate()
{

}
#pragma endregion

void BossSpider::LookPlayer()
{
	// 自機に向かうベクトル
	Vector3 targetVel = (player_->GetWorld().transform.translate - world_.transform.translate).Normalize();
	targetVel.y = 0;
	//プレイヤーの現在の向き
	targetVel = targetVel.Normalize();

	Vector3 cross = Vector3::Cross({ 0.0f,0.0f,1.0f }, targetVel).Normalize();
	float dot = Vector3::Dot({ 0.0f,0.0f,1.0f }, targetVel);

	//行きたい方向のQuaternionの作成
	world_.transform.quaternion = Quaternion::MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
}