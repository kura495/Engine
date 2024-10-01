#include "Boss.h"
#include "Game/Object/Player/Player.h"

Boss::Boss(){}
Boss::~Boss(){}

void Boss::Init(std::vector<Model*> models){

	models_ = models;
	world_.Initialize();

	world_.transform.translate.y = 5.0f;
	world_.transform.translate.z = 5.0f;
	world_.Update();
	InitCollider();

	bossWeapon_ = std::make_unique<BossWeapon>();
	bossWeapon_->Initalize(models_);
	bossWeapon_->SetParent(world_);

	HP_ = 10;
}
void Boss::Update(){
	if (HP_ <= 0) {
		IsAlive = false;
	}

	BehaviorUpdate();

	bossWeapon_->Update();
	world_.Update();
}
void Boss::Draw()
{
	models_[0]->RendererDraw(world_);
	//for (Model* model : models_) {
	//}
	bossWeapon_->Draw();
}

void Boss::LookPlayer(){
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

#pragma region
void Boss::BehaviorUpdate()
{

	if (behaviorRequest_) {
		//ふるまいの変更
		behavior_ = behaviorRequest_.value();
		//各ふるまいごとに初期化
		switch (behavior_)
		{
		case BossBehavior::kRoot:
		default:
			RootInit();
			bossWeapon_->RootInit();
			break;
		case BossBehavior::kAttack:
			AttackInit();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}
	//更新
	switch (behavior_)
	{
	case BossBehavior::kRoot:
	default:
		RootUpdate();
		bossWeapon_->RootUpdate();
		break;
	case BossBehavior::kAttack:
		AttackUpdate();

		break;
	}
}
void Boss::RootInit(){

}
void Boss::RootUpdate(){
	LookPlayer();
}
void Boss::AttackInit(){

}
void Boss::AttackUpdate(){
	AttackBehaviorUpdate();
	if (bossWeapon_->GetIsAttackOver() == true) {
		behaviorRequest_ = BossBehavior::kRoot;
	}
}
#pragma endregion Behavior

#pragma region
void Boss::AttackBehaviorUpdate(){
	if (attackbehaviorRequest_) {
		//ふるまいの変更
		attackBehavior_ = attackbehaviorRequest_.value();
		//各ふるまいごとに初期化
		switch (attackBehavior_)
		{
		case AttackBehavior::kAttack1:
		default:
			Attack1Init();
			break;
		case AttackBehavior::kAttack2:
			Attack2Init();
			break;
		}

		attackbehaviorRequest_ = std::nullopt;
	}
	//更新
	switch (attackBehavior_)
	{
	case AttackBehavior::kAttack1:
	default:
		Attack1Update();
		break;
	case AttackBehavior::kAttack2:
		Attack2Update();
		break;
	}
}
void Boss::Attack1Init(){
	bossWeapon_->AttackInit();
}
void Boss::Attack1Update(){
	bossWeapon_->AttackUpdate();
}
void Boss::Attack2Init(){

}
void Boss::Attack2Update(){

}
#pragma endregion AttackBehavior

#pragma region
void Boss::InitCollider(){
	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
	collider.OnCollision = [this](ICollider* colliderB) { OnCollision(colliderB); };
	collider.SetcollitionAttribute(ColliderTag::Enemy);
	collider.SetcollisionMask(~ColliderTag::Enemy);
}
#pragma endregion Collider