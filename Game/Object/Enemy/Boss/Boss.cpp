#include "Boss.h"
#include "Game/Object/Player/Player.h"

void Boss::Init(std::vector<Model*> models)
{
	models_ = models;

	world_.Initialize();
	//world_.transform.translate.y = 5.5f;
	worldArmL.Initialize();
	worldArmL.transform.translate = initialPosition;

	world_.Update();
	worldArmL.Update();

	//当たり判定
	colliderDamageWorld_.Initialize();
	colliderAttackWorld_.Initialize();

	ColliderDamageInit();
	ColliderAttackInit();

	//アニメーション
	animationArmLDamage = Animation::LoadAnimationFile("resources/Enemy", "Arm.gltf");
	animationArmLDamage->Init();
	animationArmLDamage->AnimeInit(*models_[Body::ArmL], false);

#pragma region 
	particle_ = new ParticleSystem();
	particle_->Initalize("resources/circle2.png");
	particle_->UpdateParticle = [this](Particle& particle) {UpdateParticle(particle); };

	deadEnemyParticleEmitter.count = 100;
	deadEnemyParticleEmitter.frequency = 0.1f;
	deadEnemyParticleEmitter.particleRadius = { 0.5f,0.5f,1.0f };
	deadEnemyParticleEmitter.color = { 1.0f,1.0f,1.0f };
	deadEnemyParticleEmitter.speed = { 2.0f,2.0f,2.0f };
#pragma endregion パーティクル

	bomb = std::make_unique<Bomb>();
	std::vector<Model*> Bombmodels;
	Bombmodels.push_back(Model::CreateModelFromObj("resources/Cube", "Cube.obj"));
	bomb->Init(Bombmodels);

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
		bomb->Draw();
		break;
	case BossBehavior::Spawn:
		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		break;
	case BossBehavior::Dead:
		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		particle_->RendererDraw();
		break;
	case BossBehavior::Down:
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
		case BossBehavior::ReturnPosition:
			ReturnPositionInit();
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
		case BossBehavior::Down:
			DownInit();
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
	case BossBehavior::ReturnPosition:
		ReturnPositionUpdate();
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
	case BossBehavior::Down:
		DownUpdate();
		break;
	}
}
void Boss::RootInit()
{
	easeT = 0.0f;

	colliderDamage.IsUsing = true;
	colliderAttack.IsUsing = true;
	colliderAttackA.IsUsing = true;
}
void Boss::RootUpdate()
{
	//TODO:消すかifdefにする
#pragma region
	//if (easeT == 1.0f) {
	//	behaviorRequest_ = BossBehavior::Dead;
	//}
	//easeT = (std::min)(easeT + 0.1f, 1.0f);
#pragma endregion デバッグ用
	//攻撃をする
	if (isAttackSelect) {
		behaviorRequest_ = BossBehavior::AttackThrowBomb;
		isAttackSelect = false;
	}
	else if(FollowPlayer()){
		behaviorRequest_ = BossBehavior::AttackSlamPlayer;
		isAttackSelect = true;
	}
}
void Boss::ReturnPositionInit()
{
	easeT = 0.0f;
	addEaseT = 0.02f;
	PrePos = worldArmL.transform.translate;
}
void Boss::ReturnPositionUpdate()
{
	if (easeT == 1.0f) {
		behaviorRequest_ = BossBehavior::Root;
	}
	
	easeT = (std::min)(easeT + addEaseT, 1.0f);
	worldArmL.transform.translate = Vector3::Lerp(PrePos, initialPosition, easeT);
	
}
void Boss::AttackSlamPlayerInit()
{
	addEaseT = 0.02f;
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
			addEaseT = 0.06f;
		}

		if (worldArmL.transform.translate.y <= 0) {
			addEaseT = 0.01f;
			colliderAttack.IsUsing = false;
			colliderAttackA.IsUsing = false;
			worldArmL.transform.translate.y = 0.5f;
			easeT = 0.0f;
			IsAttackFlag = false;
		}
	}
	else {
		//初期位置についたら
		behaviorRequest_ = BossBehavior::ReturnPosition;
	}

}
void Boss::AttackThrowBombInit()
{
	easeT = 0.0f;
	bomb->ThrowBomb(worldArmL.transform.translate,player_->GetWorld().transform.translate);
	countHitBomb = 0;
}
void Boss::AttackThrowBombUpdate()
{
	easeT = (std::min)(easeT + 0.05f, 1.0f);
	bomb->Update();

	if (countHitBomb >= 3) {
		behaviorRequest_ = BossBehavior::Down;
	}

	if (bomb->GetIsOverline()) {
		behaviorRequest_ = BossBehavior::Root;
	}
}
void Boss::SpawnInit()
{
	models_[Body::ArmL]->color_.w = 0.0f;
	worldArmL.transform.translate.z = 40.0f;
}
void Boss::SpawnUpdate()
{
	models_[Body::ArmL]->color_.w = (std::min)(models_[Body::ArmL]->color_.w + 0.01f, 1.0f);
	worldArmL.transform.translate.z = (std::max)(worldArmL.transform.translate.z - 0.1f, initialPosition.z);
	if (models_[Body::ArmL]->color_.w == 1.0f) {
		behaviorRequest_ = BossBehavior::Root;
	}
}
void Boss::DeadInit()
{
	deadEnemyParticleEmitter.world_.transform.translate = worldArmL.transform.translate;
	easeT = 0.0f;
}
void Boss::DeadUpdate()
{
	easeT = (std::min)(easeT + 0.1f, 1.0f);

	models_[Body::ArmL]->color_.w = (std::max)(models_[Body::ArmL]->color_.w - 0.01f, 0.0f);
	if (models_[Body::ArmL]->color_.w == 0.0f) {
		IsAlive = false;
	}
	particle_->Update();
}
void Boss::DownInit()
{
	colliderAttack.IsUsing = false;
	colliderAttackA.IsUsing = false;
	isDownStert = true;
	easeT = 0.0f;
	addEaseT = 0.02f;
	PrePos = worldArmL.transform.translate;
	hitCount = 0;
}
void Boss::DownUpdate()
{
	if (isDownStert) {
		animationArmLDamage->PlayAnimation();
		animationTime_ += 1.0f / 60.0f;
		if (animationTime_ > animationArmLDamage->duration) {
			isDownStert = false;
			animationTime_ = 0.0f;
		}
	}

	easeT = (std::min)(easeT + addEaseT, 1.0f);
	worldArmL.transform.translate = Vector3::Lerp(PrePos, DownPosition, easeT);

	if (hitCount == 3) {
		behaviorRequest_ = BossBehavior::ReturnPosition;
		colliderDamage.IsUsing = false;
	}
}
#pragma endregion Behavior
bool Boss::FollowPlayer()
{
	//TODO:命名仮
	Vector3 temp = player_->GetWorld().transform.translate - worldArmL.transform.translate;
	//モデルの中心から手のひらへ
	temp.z += 5.0f;
	temp.y = 0.0f;
	float playerToEnemyLngth = temp.Length();
	temp = temp.Normalize();
	worldArmL.transform.translate += temp * 0.5f;
	//TODO:命名仮
	if (playerToEnemyLngth <= 0.2f) {
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
	colliderDamage.OnCollision = [this](ICollider& colliderA) { OnCollision(colliderA); };
	colliderDamage.SetcollitionAttribute(ColliderTag::Enemy);
	colliderDamage.SetcollisionMask(~ColliderTag::EnemyAttack);
	colliderDamage.IsUsing = true;
}
void Boss::OnCollision(const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == ColliderTag::Weapon) {
		HP_ -= 1;
		hitCount += 1;
		if (HP_ <= 0) {
			behaviorRequest_ = BossBehavior::Dead;
		}
		isDamege = true;
		damegeInterval = 0;
		colliderDamage.IsUsing = false;
	}
	if (behavior_ == BossBehavior::AttackThrowBomb) {
		if (collider.GetcollitionAttribute() == ColliderTag::EnemyBomb){
			bomb->Reset(player_->GetWorld().transform.translate);
			if (easeT == 1.0f) {
				countHitBomb += 1;
				easeT = 0.0f;
			}
		}
	}

}
void Boss::ColliderAttackInit()
{
	colliderAttackWorld_.SetParent(&worldArmL);
	colliderAttack.Init(&colliderAttackWorld_);
	colliderAttack.SetSize({ 1.0f,1.0f,7.0f });
	colliderAttack.SetOffset({ 0.0f,0.0f,-2.0f });
	colliderAttack.OnCollision = [this](ICollider& colliderA) { OnCollisionAttack(colliderA); };
	colliderAttack.SetcollitionAttribute(ColliderTag::EnemyAttack);
	colliderAttack.SetcollisionMask(~ColliderTag::Enemy);

	colliderAttackA.Init(&colliderAttackWorld_);
	colliderAttackA.SetSize({ 2.0f,0.5f,1.0f });
	colliderAttackA.SetOffset({ 0.0f,0.0f,-6.25f });
	colliderAttackA.OnCollision = [this](ICollider& colliderA) { OnCollisionAttack(colliderA); };
	colliderAttackA.SetcollitionAttribute(ColliderTag::EnemyAttack);
	colliderAttackA.SetcollisionMask(~ColliderTag::Enemy);	
	colliderAttack.IsUsing = true;
	colliderAttackA.IsUsing = true;
}
void Boss::OnCollisionAttack(const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == ColliderTag::Player) {
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