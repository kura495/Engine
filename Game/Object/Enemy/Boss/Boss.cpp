#include "Boss.h"

void Boss::Init(std::vector<Model*> models)
{
	models_ = models;

	worldArmL.Initialize();
	worldArmL.transform.translate.y = 5.5f;

	ColliderDamegeWorld_.Initialize();
	colliderAttackWorld_.Initialize();
	world_.Initialize();
	world_.Update();
	worldArmL.Update();

	colliderDamageInit();
	colliderAttackInit();

	animationArmLDamage = Animation::LoadAnimationFile("resources/Enemy", "Arm.gltf");
	animationArmLDamage->Init();
	animationArmLDamage->AnimeInit(*models_[Body::ArmL], false);

	behaviorRequest_ = BossBehavior::kRoot;

	name = "Boss";

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

	world_.Update();
	worldArmL.Update();
}

void Boss::Draw()
{
	switch (behavior_)
	{
	case BossBehavior::kRoot:
	default:
		//models_[Body::body]->RendererDraw(world_);
		models_[Body::ArmL]->RendererDraw(worldArmL);
		//models_[Body::ArmR]->RendererSkinDraw(worldArmR, animationArmRRoot->GetSkinCluster());
		break;
	case BossBehavior::kAttackL:
		//models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
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
	easeT = 0.0f;
}
void Boss::kRootUpdate()
{
	//攻撃をする
	if (/*条件*/true) {
		//behaviorRequest_ = BossBehavior::kAttackL;
	}
}
void Boss::kAttackLInit()
{
	addEaseT = 0.01f;
}
void Boss::kAttackLUpdate()
{

	easeT = (std::min)(easeT + addEaseT,1.0f);

	worldArmL.transform.translate.y -= Ease::InBack(easeT);
	float aaa = Ease::InBack(easeT);

	if (aaa > 0){
		addEaseT = 0.05f;
	}

	if (worldArmL.transform.translate.y <= 0){
		worldArmL.transform.translate.y = 0.5f;
		behaviorRequest_ = BossBehavior::kRoot;
	}
}
void Boss::kAttackRInit()
{
}
void Boss::kAttackRUpdate()
{
}
#pragma endregion Behavior
#pragma region
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
void Boss::AddImGui()
{
	if (ImGui::Button("AttackMove")) {
		behaviorRequest_ = BossBehavior::kAttackL;
	}
	ImGui::DragFloat3("Scale", &worldArmL.transform.scale.x);
	ImGui::DragFloat4("Rotate", &worldArmL.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &worldArmL.transform.translate.x);
}