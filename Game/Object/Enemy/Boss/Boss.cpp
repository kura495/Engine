#include "Boss.h"

void Boss::Init(std::vector<Model*> models)
{
	models_ = models;

	worldArmL.Initialize();
	worldArmL.transform.translate.y = 5.5f;

	colliderDamegeWorld_.Initialize();
	colliderAttackWorld_.Initialize();
	world_.Initialize();
	world_.transform.translate.y = 5.5f;
	world_.Update();
	worldArmL.Update();

	ColliderDamageInit();
	ColliderAttackInit();

	animationArmLDamage = Animation::LoadAnimationFile("resources/Enemy", "Arm.gltf");
	animationArmLDamage->Init();
	animationArmLDamage->AnimeInit(*models_[Body::ArmL], false);

	behaviorRequest_ = BossBehavior::Root;

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
	case BossBehavior::Root:
	default:
		//models_[Body::body]->RendererDraw(world_);
		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		//models_[Body::ArmR]->RendererSkinDraw(worldArmR, animationArmRRoot->GetSkinCluster());
		break;
	case BossBehavior::AttackL:
		//models_[Body::ArmL]->RendererDraw(worldArmL);

		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		//models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		break;
	case BossBehavior::AttackR:
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
		case BossBehavior::Root:
		default:
			RootInit();
			break;
		case BossBehavior::AttackL:
			AttackLInit();
			break;
		case BossBehavior::AttackR:
			AttackRInit();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}
	//更新
	switch (behavior_)
	{
	case BossBehavior::Root:
	default:
		RootUpdate();
		break;
	case BossBehavior::AttackL:
		AttackLUpdate();
		break;
	case BossBehavior::AttackR:
		AttackRUpdate();
		break;
	}
}
void Boss::RootInit()
{
	easeT = 0.0f;
}
void Boss::RootUpdate()
{
	
	if (worldArmL.transform.translate.y < 5.5f) {
		easeT = (std::min)(easeT + 0.01f, 1.0f);
		worldArmL.transform.translate.y = Vector3::Lerp({0.0f,0.0f,0.0f},{0.0f,5.5f,0.0f}, easeT).y;

	}
	//攻撃をする
	if (worldArmL.transform.translate.y >= 5.5f) {
		behaviorRequest_ = BossBehavior::AttackL;
	}
}
void Boss::AttackLInit()
{
	addEaseT = 0.01f;
	colliderAttack.IsUsing = true;
	colliderAttackA.IsUsing = true;
	easeT = 0.0f;
}
void Boss::AttackLUpdate()
{

	easeT = (std::min)(easeT + addEaseT, 1.0f);

	worldArmL.transform.translate.y -= Ease::InBack(easeT);
	float newPoint = Ease::InBack(easeT);

	if (newPoint > 0) {
		addEaseT = 0.05f;
	}

	if (worldArmL.transform.translate.y <= 0) {
		colliderAttack.IsUsing = false;
		colliderAttackA.IsUsing = false;
		worldArmL.transform.translate.y = 0.5f;
		behaviorRequest_ = BossBehavior::Root;
	}
}
void Boss::AttackRInit()
{
}
void Boss::AttackRUpdate()
{
}
#pragma endregion Behavior
#pragma region
void Boss::ColliderDamageInit()
{
	colliderDamegeWorld_.SetParent(&worldArmL);

	colliderDamage.Init(&colliderDamegeWorld_);
	colliderDamage.SetSize({ 1.0f,1.0f,1.0f });
	colliderDamage.OnCollision = [this](ICollider* colliderA) { OnCollision(colliderA); };
	colliderDamage.SetcollitionAttribute(ColliderTag::Enemy);
	colliderDamage.SetcollisionMask(~ColliderTag::EnemyAttack);
	colliderDamage.IsUsing = true;
}
void Boss::OnCollision(const ICollider* collider)
{
	if (collider->GetcollitionAttribute() == ColliderTag::Weapon) {
		HP_ -= 1;
		isDamege = true;
		damegeInterval = 0;
		colliderDamage.IsUsing = false;
	}
}
void Boss::ColliderAttackInit()
{
	colliderAttackWorld_.SetParent(&worldArmL);
	colliderAttack.Init(&colliderAttackWorld_);
	colliderAttack.SetSize({ 1.0f,1.0f,7.0f });
	colliderAttack.SetOffset({ 0.0f,0.0f,-2.0f });
	colliderAttack.OnCollision = [this](ICollider* colliderA) { OnCollisionAttack(colliderA); };
	colliderAttack.SetcollitionAttribute(ColliderTag::EnemyAttack);
	colliderAttack.SetcollisionMask(~ColliderTag::Enemy);

	colliderAttackA.Init(&colliderAttackWorld_);
	colliderAttackA.SetSize({ 2.0f,0.5f,1.0f });
	colliderAttackA.SetOffset({ 0.0f,0.0f,-6.25f });
	colliderAttackA.OnCollision = [this](ICollider* colliderA) { OnCollisionAttack(colliderA); };
	colliderAttackA.SetcollitionAttribute(ColliderTag::EnemyAttack);
	colliderAttackA.SetcollisionMask(~ColliderTag::Enemy);	
	colliderAttack.IsUsing = false;
	colliderAttackA.IsUsing = false;
}
void Boss::OnCollisionAttack(const ICollider* collider)
{
	if (collider->GetcollitionAttribute() == ColliderTag::Player) {
		colliderAttack.IsUsing = false;
		colliderAttackA.IsUsing = false;
	}
}
#pragma endregion Collider
void Boss::AddImGui()
{
	if (ImGui::Button("AttackMove")) {
		behaviorRequest_ = BossBehavior::AttackL;
	}
	ImGui::DragFloat3("Scale", &worldArmL.transform.scale.x);
	ImGui::DragFloat4("Rotate", &worldArmL.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &worldArmL.transform.translate.x);
}