#include "Boss.h"
#include "Object/Player/Player.h"

void Boss::Init(std::vector<Model*> models)
{
	models_ = models;
	//ワールド初期化
	world_.Init();
	//当たり判定
	ColliderInit();
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
			colliders_[Boss::ColliderType::DamageArm].IsUsing = true;
			colliders_[Boss::ColliderType::DamageHund].IsUsing = true;

		}
	}

	state_->Update(this);

	world_.Update();
}
void Boss::Draw()
{
	state_->Draw(this);
}
void Boss::ReStert()
{
	ChangeState<EReStert>();
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
void Boss::ColliderInit()
{
	colliderWorld_.Init();
	colliderWorld_.SetParent(&world_);
	ColliderDamageInit();
	ColliderAttackInit();
}
void Boss::ColliderDamageInit()
{
	//腕側の攻撃判定
	colliders_[Boss::ColliderType::DamageArm].Init(&colliderWorld_);
	colliders_[Boss::ColliderType::DamageArm].SetSize(armColliderSize);
	colliders_[Boss::ColliderType::DamageArm].SetOffset(armColliderOffset);
	colliders_[Boss::ColliderType::DamageArm].OnCollision = [this](ICollider& colliderA) { OnCollisionDamage(colliderA); };
	colliders_[Boss::ColliderType::DamageArm].SetcollitionAttribute(Collider::Tag::EnemyCore);
	colliders_[Boss::ColliderType::DamageArm].SetcollisionMask(~Collider::Tag::EnemyAttack & ~Collider::Tag::EnemyAttackFront);
	//指側の攻撃判定
	colliders_[Boss::ColliderType::DamageHund].Init(&colliderWorld_);
	colliders_[Boss::ColliderType::DamageHund].SetSize(hundColliderSize);
	colliders_[Boss::ColliderType::DamageHund].SetOffset(hundColliderOffset);
	colliders_[Boss::ColliderType::DamageHund].OnCollision = [this](ICollider& colliderA) { OnCollisionDamage(colliderA); };
	colliders_[Boss::ColliderType::DamageHund].SetcollitionAttribute(Collider::Tag::EnemyCore);
	colliders_[Boss::ColliderType::DamageHund].SetcollisionMask(~Collider::Tag::EnemyAttack & ~Collider::Tag::EnemyAttackFront);

	colliders_[Boss::ColliderType::DamageArm].colliderName = "ColliderDamageArm";
	colliders_[Boss::ColliderType::DamageHund].colliderName = "ColliderDamageHund";
}
void Boss::OnCollisionDamage(const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == Collider::Tag::Weapon) {
		HP_ -= 1;
		if (HP_ <= 0) {
			ChangeState<EDead>();
		}
		isDamage = true;
		colliders_[Boss::ColliderType::DamageArm].IsUsing = false;
		colliders_[Boss::ColliderType::DamageHund].IsUsing = false;
	}

	state_->OnCollisionDamage(this,collider);
	
}
void Boss::ColliderAttackInit()
{
	//腕側の攻撃判定
	colliders_[Boss::ColliderType::AttackArm].Init(&colliderWorld_);
	colliders_[Boss::ColliderType::AttackArm].SetSize(armColliderSize);
	colliders_[Boss::ColliderType::AttackArm].SetOffset(armColliderOffset);
	colliders_[Boss::ColliderType::AttackArm].OnCollision = [this](ICollider& colliderA) { OnCollisionAttack(colliderA); };
	colliders_[Boss::ColliderType::AttackArm].SetcollitionAttribute(Collider::Tag::EnemyAttack);
	colliders_[Boss::ColliderType::AttackArm].SetcollisionMask(~Collider::Tag::EnemyCore & ~Collider::Tag::EnemyAttackFront);
	//指側の攻撃判定
	colliders_[Boss::ColliderType::AttackHund].Init(&colliderWorld_);
	colliders_[Boss::ColliderType::AttackHund].SetSize(hundColliderSize);
	colliders_[Boss::ColliderType::AttackHund].SetOffset(hundColliderOffset);
	colliders_[Boss::ColliderType::AttackHund].OnCollision = [this](ICollider& colliderA) { OnCollisionAttack(colliderA); };
	colliders_[Boss::ColliderType::AttackHund].SetcollitionAttribute(Collider::Tag::EnemyAttack);
	colliders_[Boss::ColliderType::AttackHund].SetcollisionMask(~Collider::Tag::EnemyCore);


	colliders_[Boss::ColliderType::AttackArm].colliderName = "ColliderAttackArm";
	colliders_[Boss::ColliderType::AttackHund].colliderName = "ColliderAttackHund";
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
	if (ImGui::Button("Slam")) {
		ChangeState<EAttackSlam>();
	}
	ImGui::Text(state_->ShowState().c_str());
}
