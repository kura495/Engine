#include "Boss.h"
#include "Game/Object/Player/Player.h"

void Boss::Init(std::vector<Model*> models)
{
	models_ = models;
	//ワールド初期化
	world_.Initialize();
	world_.Update();
	worldArmL.Initialize();
	worldArmL.transform.translate = initialPosition;
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
	particle_->Initalize("resources/circle2.dds");
	particle_->UpdateParticle = [this](Particle& particle) { UpdateParticle(particle); };
	particle_->CustumSpawn = [this]() { return CustomParticle(); };

	deadEnemyParticleEmitter.count = 50;
	deadEnemyParticleEmitter.frequency = 0.1f;
	deadEnemyParticleEmitter.particleRadius = { 1.0f,1.0f,1.0f };
	deadEnemyParticleEmitter.color = { 1.0f,1.0f,1.0f };
	deadEnemyParticleEmitter.speed = { 2.0f,2.0f,2.0f };
#pragma endregion パーティクル
#pragma region
	ball = std::make_unique<Ball>();
	std::vector<Model*> ballmodels;
	ballmodels.push_back(Model::CreateModelFromObj("resources/Cube", "Cube.obj"));
	ball->Init(ballmodels);

	dummyBall = std::make_unique<DummyBall>();
	dummyBall->Init(ballmodels);
#pragma endregion ボール
	//ビヘイビアーを初期化
	behaviorRequest_ = BossBehavior::Spawn;

	models_[Body::ArmL]->color_.w = 0.0f;

	name = "Boss";
	//初期値を設定
	HP_ = 9;
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
	case BossBehavior::AttackThrowball:
		models_[Body::ArmL]->RendererSkinDraw(worldArmL, animationArmLDamage->GetSkinCluster());
		ball->Draw();
		dummyBall->Draw();
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
		case BossBehavior::AttackThrowball:
			AttackThrowBallInit();
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
	case BossBehavior::AttackThrowball:
		AttackThrowBallUpdate();
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
	//当たり判定を有効か
	colliderDamage.IsUsing = true;
	colliderAttack.IsUsing = true;
	colliderAttackA.IsUsing = true;
	//当たり判定を通常に変更
	colliderAttack.SetcollitionAttribute(ColliderTag::Enemy);
	colliderAttackA.SetcollitionAttribute(ColliderTag::Enemy);
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
		//ボールを投げる攻撃
		behaviorRequest_ = BossBehavior::AttackThrowball;
		isAttackSelect = false;
	}
	else if (FollowPlayer()) {
		//プレイヤーを叩きつけ攻撃
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
	if (easeT >= 0.2f) {
		isSlamFlag = false;
	}
	//
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
	//当たり判定を攻撃に変更
	colliderAttack.SetcollitionAttribute(ColliderTag::EnemyAttack);
	colliderAttackA.SetcollitionAttribute(ColliderTag::EnemyAttack);
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
		//位置が0になったら
		if (worldArmL.transform.translate.y <= 0) {
			isSlamFlag = true;
			addEaseT = 0.01f;
			colliderAttack.IsUsing = false;
			colliderAttackA.IsUsing = false;
			worldArmL.transform.translate.y = 0.5f;
			easeT = 0.0f;
			IsAttackFlag = false;
		}
	}
	else {
		//2回目移行かつHPが低くなった時に処理を実行
		if (isSlam2ndFlag && HP_ <= 3) {
			//falseにすることで2回連続で叩きつけをするようにする
			isAttackSelect = false;
			isSlam2ndFlag = false;
		}
		else if (isSlam2ndFlag == false) {
			isSlam2ndFlag = true;
		}
		//初期位置に戻す
		behaviorRequest_ = BossBehavior::ReturnPosition;

	}

}
void Boss::AttackThrowBallInit()
{
	easeT = 0.0f;
	ball->ThrowBall(worldArmL.transform.translate, player_->GetWorld().transform.translate);
	isThrowdummyBallFlag = false;
	countHitBall = 0;
}
void Boss::AttackThrowBallUpdate()
{
	easeT = (std::min)(easeT + 0.05f, 1.0f);
	ball->Update();
	dummyBall->Update();
	//跳ね返しのタイミングでもう一つの球を出す
	if (countHitBall == 1 && isThrowdummyBallFlag == false) {
		dummyBall->ThrowBall(worldArmL.transform.translate, { worldArmL.transform.translate.x + 5.0f,worldArmL.transform.translate.y,worldArmL.transform.translate.z }, player_->GetWorld().transform.translate);
		isThrowdummyBallFlag = true;
	}
	//3回球に当たったらやられ状態にする
	if (countHitBall >= 3) {
		dummyBall->Reset();
		behaviorRequest_ = BossBehavior::Down;
	}
	//ボールが一定のラインを超えたらルートビヘイビアーに戻す
	if (ball->GetIsOverline()) {
		dummyBall->Reset();
		behaviorRequest_ = BossBehavior::Root;
	}
}
void Boss::SpawnInit()
{
	models_[Body::ArmL]->color_.w = 0.0f;
	worldArmL.transform.translate.z = 40.0f;
	//当たり判定を通常に変更
	colliderAttack.SetcollitionAttribute(ColliderTag::Enemy);
	colliderAttackA.SetcollitionAttribute(ColliderTag::Enemy);
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

	//パーティクル生成
	deadEnemyParticleEmitter.frequencyTime += kDeltaTime;
	if (deadEnemyParticleEmitter.frequency <= deadEnemyParticleEmitter.frequencyTime) {
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());

		particle_->SpawnParticle(deadEnemyParticleEmitter, randomEngine);

		deadEnemyParticleEmitter.frequencyTime -= deadEnemyParticleEmitter.frequency;
	}
	//パーティクル更新
	particle_->Update();
}
void Boss::DownInit()
{
	colliderAttack.IsUsing = true;
	colliderAttackA.IsUsing = true;
	//当たり判定を通常に変更
	colliderAttack.SetcollitionAttribute(ColliderTag::Enemy);
	colliderAttackA.SetcollitionAttribute(ColliderTag::Enemy);

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
	//3回攻撃を受けると元の位置に戻す
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
	colliderDamage.SetcollitionAttribute(ColliderTag::EnemyCore);
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
	if (behavior_ == BossBehavior::AttackThrowball) {
		if (collider.GetcollitionAttribute() == ColliderTag::EnemyBall) {
			ball->Reset(player_->GetWorld().transform.translate);
			if (easeT == 1.0f) {
				countHitBall += 1;
				easeT = 0.0f;
			}
		}
	}

}
void Boss::ColliderAttackInit()
{
	colliderAttackWorld_.SetParent(&worldArmL);
	colliderAttack.Init(&worldArmL);
	colliderAttack.SetSize({ 1.0f,1.0f,7.0f });
	colliderAttack.SetOffset({ 0.0f,0.0f,-2.0f });
	colliderAttack.OnCollision = [this](ICollider& colliderA) { OnCollisionAttack(colliderA); };
	colliderAttack.SetcollitionAttribute(ColliderTag::EnemyAttack);
	colliderAttack.SetcollisionMask(~ColliderTag::EnemyCore);

	colliderAttackA.Init(&worldArmL);
	colliderAttackA.SetSize({ 2.0f,0.5f,1.0f });
	colliderAttackA.SetOffset({ 0.0f,0.0f,-6.25f });
	colliderAttackA.OnCollision = [this](ICollider& colliderA) { OnCollisionAttack(colliderA); };
	colliderAttackA.SetcollitionAttribute(ColliderTag::EnemyAttack);
	colliderAttackA.SetcollisionMask(~ColliderTag::EnemyCore);
	colliderAttack.IsUsing = true;
	colliderAttackA.IsUsing = true;
}
void Boss::OnCollisionAttack(const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == ColliderTag::Player && colliderAttack.GetcollisionMask() == ColliderTag::EnemyAttack) {
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
	//ImGui::DragFloat3("Scale", &worldArmL.transform.scale.x);
	//ImGui::DragFloat4("Rotate", &worldArmL.transform.quaternion.x);
	ImGui::DragFloat3("ParticleTranslate", &deadEnemyParticleEmitter.world_.transform.translate.x);
}

void Boss::UpdateParticle(Particle& particle)
{

	Vector3 velcity = particle.velocity * kDeltaTime;
	particle.transform.translate += velcity * deadEnemyParticleEmitter.speed;
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = deadEnemyParticleEmitter.particleRadius;
	Vector3 translate = particle.transform.translate + deadEnemyParticleEmitter.world_.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	particle.color.w = alpha;
	particle.color.x = deadEnemyParticleEmitter.color.x;
	particle.color.y = deadEnemyParticleEmitter.color.y;
	particle.color.z = deadEnemyParticleEmitter.color.z;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);

}

Particle Boss::CustomParticle()
{
	Particle particle{};
	particle.color = { 1.0f,1.0f,1.0f };
	particle.currentTime = 0.0f;
	particle.lifeTime = 2.0f;
	particle.transform.translate = worldArmL.transform.translate;

	return particle;
}
