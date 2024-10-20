#include "Boss.h"

void Boss::Init(std::vector<Model*> models)
{
	models_ = models;

	worldArmL.Initialize();
	worldArmL.transform.translate.y = 0.5f;

	ColliderDamegeWorld_.Initialize();
	colliderAttackWorld_.Initialize();
	world_.Initialize();
	world_.Update();
	worldArmL.Update();

	colliderDamageInit();
	colliderAttackInit();
	InitCollider();

	animationArmLDamage = Animation::LoadAnimationFile("resources/Enemy", "Arm.gltf");
	animationArmLDamage->Init();
	animationArmLDamage->AnimeInit(*models_[Body::ArmL], false);

	behaviorRequest_ = BossBehavior::kAttackL;

}

void Boss::Update()
{
	//ダメージを受けた時
	if (isDamege) {
		animationArmLDamage->PlayAnimation();
		animationTime_ += 1.0f / 60.0f;
		if (animationTime_ > animationArmLDamage->duration) {
			isDamege = false;
			animationTime_ = 0.0f;
			colliderDamage.IsUsing = true;
		}
	}

	BehaviorUpdate();
}

void Boss::Draw()
{
	switch (behavior_)
	{
	case BossBehavior::kRoot:
	default:
		models_[Body::body]->RendererDraw(world_);
		models_[Body::ArmL]->RendererDraw(worldArmL);
		//models_[Body::ArmR]->RendererSkinDraw(worldArmR, animationArmRRoot->GetSkinCluster());
		break;
	case BossBehavior::kAttackL:
		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		break;
	case BossBehavior::kAttackR:
		//models_[Body::ArmR]->RendererSkinDraw(worldArmR, animationArmRDamage->GetSkinCluster());
		break;
	}

}
#pragma region
void Boss::BehaviorUpdate()
{
	//初期化
	if (behaviorRequest_) {
		//ふるまいの変更
		behavior_ = behaviorRequest_.value();
		//各ふるまいごとに初期化
		switch (behavior_)
		{
		case BossBehavior::kRoot:
		default:
			kRootInit();
			break;
		case BossBehavior::kAttackL:
			kAttackLInit();
			break;
		case BossBehavior::kAttackR:
			kAttackRInit();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}
	//更新
	switch (behavior_)
	{
	case BossBehavior::kRoot:
	default:
		kRootUpdate();
		break;
	case BossBehavior::kAttackL:
		kAttackLUpdate();
		break;
	case BossBehavior::kAttackR:
		kAttackRUpdate();
		break;
	}
}
void Boss::kRootInit()
{
}
void Boss::kRootUpdate()
{
}
void Boss::kAttackLInit()
{
}
void Boss::kAttackLUpdate()
{

}
void Boss::kAttackRInit()
{
}
void Boss::kAttackRUpdate()
{
}
#pragma endregion Behavior
#pragma region
void Boss::InitCollider()
{
	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
	collider.OnCollision = [this](ICollider* colliderB) { OnCollision(colliderB); };
	collider.SetcollitionAttribute(ColliderTag::Enemy);
	collider.SetcollisionMask(~ColliderTag::Enemy);
	collider.IsUsing = false;
}
void Boss::colliderDamageInit()
{
	ColliderDamegeWorld_.SetParent(&worldArmL);

	colliderDamage.Init(&ColliderDamegeWorld_);
	colliderDamage.SetSize({ 1.0f,1.0f,1.0f });
	colliderDamage.OnCollision = [this](ICollider* colliderA) { OnCollision(colliderA); };
	colliderDamage.SetcollitionAttribute(ColliderTag::Enemy);
	colliderDamage.SetcollisionMask(~ColliderTag::EnemyAttack);
	colliderDamage.IsUsing = true;
}
void Boss::OnCollision(const ICollider* colliderA)
{
	if (colliderA->GetcollitionAttribute() == ColliderTag::Weapon) {
		HP_ -= 1;
		isDamege = true;
		damegeInterval = 0;
		colliderDamage.IsUsing = false;
	}
}
void Boss::colliderAttackInit()
{
	colliderAttackWorld_.SetParent(&worldArmL);
	colliderAttackWorld_.transform.translate.y = 0.5f;
	colliderAttackWorld_.transform.translate.z = 1.0f;
	colliderAttack.Init(&colliderAttackWorld_);
	colliderAttack.SetSize({ 0.5f,1.0f,0.5f });
	colliderAttack.OnCollision = [this](ICollider* colliderA) { onCollisionAttack(colliderA); };
	colliderAttack.SetcollitionAttribute(ColliderTag::EnemyAttack);
	colliderAttack.SetcollisionMask(~ColliderTag::Enemy);
	colliderAttack.IsUsing = false;
}
void Boss::onCollisionAttack(const ICollider* Collider)
{
	Collider;
}
#pragma endregion Collider
void Boss::Damage()
{
	//hpを減らす
	HP_ = -1;


}
