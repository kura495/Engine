#include "Boss.h"
#include "Game/Object/Player/Player.h"

void Boss::Init(std::vector<Model*> models)
{
	models_ = models;

	world_.Initialize();
	world_.transform.translate.y = 5.5f;
	worldArmL.Initialize();
	worldArmL.transform.translate.y = 5.5f;

	colliderDamageWorld_.Initialize();
	colliderAttackWorld_.Initialize();

	world_.Update();
	worldArmL.Update();

	ColliderDamageInit();
	ColliderAttackInit();

	animationArmLDamage = Animation::LoadAnimationFile("resources/Enemy", "Arm.gltf");
	animationArmLDamage->Init();
	animationArmLDamage->AnimeInit(*models_[Body::ArmL], false);
	
	//animationSpawn = Animation::LoadAnimationFile("resources/Enemy", "Arm.gltf");
	//animationSpawn->Init();
	//animationSpawn->AnimeInit(*models_[Body::ArmL], false);

	behaviorRequest_ = BossBehavior::Spawn;

	models_[Body::ArmL]->color_.w = 0.0f;

	name = "Boss";
	//初期値を設定
	HP_ = 1;
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
		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		break;
	case BossBehavior::AttackSlamPlayer:
		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		break;
	case BossBehavior::AttackThrowBomb:
		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		break;
	case BossBehavior::Spawn:
		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		break;
	case BossBehavior::Dead:
		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
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
		case BossBehavior::AttackSlamPlayer:
			AttackSlamPlayerInit();
			break;
		case BossBehavior::AttackThrowBomb:
			AttackThrowBombInit();
			break;
		case BossBehavior::Spawn:
			SpawnInit();
			break;
		case BossBehavior::Dead:
			DeadInit();
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
	case BossBehavior::AttackSlamPlayer:
		AttackSlamPlayerUpdate();
		break;
	case BossBehavior::AttackThrowBomb:
		AttackThrowBombUpdate();
		break;
	case BossBehavior::Spawn:
		SpawnUpdate();
		break;
	case BossBehavior::Dead:
		DeadUpdate();
		break;
	}
}
void Boss::RootInit()
{
	easeT = 0.0f;
}
void Boss::RootUpdate()
{
	//攻撃をする
	if (FollowPlayer()) {
		behaviorRequest_ = BossBehavior::AttackSlamPlayer;
	}
}
void Boss::AttackSlamPlayerInit()
{
	addEaseT = 0.01f;
	colliderAttack.IsUsing = true;
	colliderAttackA.IsUsing = true;
	IsAttackFlag = true;
	easeT = 0.0f;
}
void Boss::AttackSlamPlayerUpdate()
{
	if (IsAttackFlag) {
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
			easeT = 0.0f;
			IsAttackFlag = false;
		}
	}
	else {
		//手を位置に戻す
		if (worldArmL.transform.translate.y < 5.5f) {
			easeT = (std::min)(easeT + 0.01f, 1.0f);
			worldArmL.transform.translate.y = Vector3::Lerp({0.0f,0.0f,0.0f},{0.0f,5.5f,0.0f}, easeT).y;
		}
		//初期位置についたら
		else if (worldArmL.transform.translate.y >= 5.5f) {
			behaviorRequest_ = BossBehavior::Root;
		}
	}

}
void Boss::AttackThrowBombInit()
{

}
void Boss::AttackThrowBombUpdate()
{

}
void Boss::SpawnInit()
{
	models_[Body::ArmL]->color_.w = 0.0f;
	worldArmL.transform.translate.z = 10.0f;
}
void Boss::SpawnUpdate()
{
	models_[Body::ArmL]->color_.w = (std::min)(models_[Body::ArmL]->color_.w + 0.01f, 1.0f);
	worldArmL.transform.translate.z = (std::max)(worldArmL.transform.translate.z - 0.1f, 5.0f);
	if (models_[Body::ArmL]->color_.w == 1.0f) {
		behaviorRequest_ = BossBehavior::Root;
	}
}
void Boss::DeadInit()
{

}
void Boss::DeadUpdate()
{
	models_[Body::ArmL]->color_.w = (std::max)(models_[Body::ArmL]->color_.w - 0.01f, 0.0f);
	if (models_[Body::ArmL]->color_.w == 0.0f) {
		IsAlive = false;
	}
}
#pragma endregion Behavior
bool Boss::FollowPlayer()
{
	//TODO:命名仮
	Vector3 temp = player_->GetWorld().transform.translate - worldArmL.transform.translate;
	//モデルの中心から手のひらへ
	temp.z += 10.0f;
	temp.y = 0.0f;
	float playerToEnemyLngth = temp.Length();
	temp = temp.Normalize();
	worldArmL.transform.translate += temp * 0.2f;
	//TODO:命名仮
	if (playerToEnemyLngth <= 1.5f) {
		return true;
	}
	return false;
}

#pragma region
void Boss::ColliderDamageInit()
{
	colliderDamageWorld_.SetParent(&worldArmL);

	colliderDamage.Init(&colliderDamageWorld_);
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
		if (HP_ <= 0) {
			behaviorRequest_ = BossBehavior::Dead;
		}
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
		behaviorRequest_ = BossBehavior::AttackSlamPlayer;
	}
	ImGui::DragFloat3("Scale", &worldArmL.transform.scale.x);
	ImGui::DragFloat4("Rotate", &worldArmL.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &worldArmL.transform.translate.x);
}