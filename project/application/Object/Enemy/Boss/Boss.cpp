#include "Boss.h"
#include "Object/Player/Player.h"

void Boss::Init(std::vector<Model*> models)
{
	models_ = models;
	//ワールド初期化
	world_.Init();
	//当たり判定
	colliderDamageWorld_.Init();
	colliderAttackWorld_.Init();
	ColliderDamageInit();
	ColliderAttackInit();
	//アニメーション
	animationArmLDamage = std::make_unique<Animation>();
	animationArmLDamage.reset(Animation::LoadAnimationFile("project/resources/Enemy", "Arm.gltf"));
	animationArmLDamage->Init();
	animationArmLDamage->AnimeInit(*models_[BossModel::MainBody], false);
	//ビヘイビアーを初期化
	ChangeState<ESpawn>();
	name = "Boss";
	//初期値を設定
	HP_ = 10;
}
void Boss::Update()
{
	//ダメージを受けた時
	if (isDamage) {
		animationArmLDamage->PlayAnimation();
		animationTime_ += kDeltaTime;
		if (animationTime_ > animationArmLDamage->duration) {
			isDamage = false;
			animationTime_ = kDeltaTime;
			colliders_[Boss::ColliderType::WeekPoint].IsUsing = true;

		}
	}

	state_->Update(this);

	world_.Update();
}
void Boss::Draw()
{
	state_->Draw(this);
}
void Boss::SetColliderUse(int number, bool flag)
{
	colliders_[number].IsUsing = flag;
}
void Boss::SetColliderAttribute(int number, uint32_t collisionAttribute)
{
	colliders_[number].SetcollitionAttribute(collisionAttribute);
}
#pragma region
void Boss::ColliderDamageInit()
{
	colliderDamageWorld_.SetParent(&world_);
	colliders_[Boss::ColliderType::WeekPoint].Init(&colliderDamageWorld_);
	colliders_[Boss::ColliderType::WeekPoint].SetSize({ 1.0f,1.0f,1.0f });
	colliders_[Boss::ColliderType::WeekPoint].OnCollision = [this](ICollider& colliderA) { OnCollision(colliderA); };
	colliders_[Boss::ColliderType::WeekPoint].SetcollitionAttribute(Collider::Tag::EnemyCore);
	colliders_[Boss::ColliderType::WeekPoint].SetcollisionMask(~Collider::Tag::EnemyAttack & ~Collider::Tag::EnemyAttackFront);
	colliders_[Boss::ColliderType::WeekPoint].IsUsing = true;
}
void Boss::OnCollision(const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == Collider::Tag::Weapon) {
		HP_ -= 1;
		if (HP_ <= 0) {
			ChangeState<EDead>();
		}
		isDamage = true;
		damageInterval = 0;
		colliders_[Boss::ColliderType::WeekPoint].IsUsing = false;
	}

	state_->OnCollision(this,collider);
	
}
void Boss::ColliderAttackInit()
{
	//腕側の攻撃判定
	colliderAttackWorld_.SetParent(&world_);
	colliders_[Boss::ColliderType::Arm].Init(&colliderAttackWorld_);
	colliders_[Boss::ColliderType::Arm].SetSize({ 1.0f,1.0f,7.0f });
	colliders_[Boss::ColliderType::Arm].SetOffset({ 0.0f,0.0f,-2.0f });
	colliders_[Boss::ColliderType::Arm].OnCollision = [this](ICollider& colliderA) { OnCollisionAttack(colliderA); };
	colliders_[Boss::ColliderType::Arm].SetcollitionAttribute(Collider::Tag::EnemyAttack);
	colliders_[Boss::ColliderType::Arm].SetcollisionMask(~Collider::Tag::EnemyCore & ~Collider::Tag::EnemyAttackFront);
	//指側の攻撃判定
	colliders_[Boss::ColliderType::Hund].Init(&colliderAttackWorld_);
	colliders_[Boss::ColliderType::Hund].SetSize({ 2.0f,0.5f,1.0f });
	colliders_[Boss::ColliderType::Hund].SetOffset({ 0.0f,0.0f,-6.25f });
	colliders_[Boss::ColliderType::Hund].OnCollision = [this](ICollider& colliderA) { OnCollisionAttack(colliderA); };
	colliders_[Boss::ColliderType::Hund].SetcollitionAttribute(Collider::Tag::EnemyAttack);
	colliders_[Boss::ColliderType::Hund].SetcollisionMask(~Collider::Tag::EnemyCore);
}
void Boss::OnCollisionAttack(const ICollider& collider)
{
	state_->OnCollisionAttack(this,collider);
}
#pragma endregion Collider
void Boss::AddImGui()
{
	if (ImGui::Button("Sword")) {
		ChangeState<ESwordAttack>();
	}
	ImGui::Text(state_->ShowState().c_str());
}
