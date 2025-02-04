#include "Boss.h"
#include "Object/Player/Player.h"

void Boss::Init(std::vector<Model*> models)
{
	models_ = models;
	//ワールド初期化
	world_.Init();
	world_.transform.translate = DownPosition;
	world_.Update();
	//当たり判定
	colliderDamageWorld_.Init();
	colliderAttackWorld_.Init();
	ColliderDamageInit();
	ColliderAttackInit();
	//アニメーション
	animationArmLDamage = Animation::LoadAnimationFile("project/resources/Enemy", "Arm.gltf");
	animationArmLDamage->Init();
	animationArmLDamage->AnimeInit(*models_[0], false);
#pragma region 
	deadParticle_ = new ParticleSystem();
	deadParticle_->Init("project/resources/circle2.dds");
	deadParticle_->UpdateFunc = [this](Particle& particle) { UpdateParticle(particle); };

	deadEnemyParticleEmitter.count = 50;
	deadEnemyParticleEmitter.frequency = 0.1f;
	deadEnemyParticleEmitter.particleRadius = { 1.0f,1.0f,1.0f };
	deadEnemyParticleEmitter.color = { 1.0f,1.0f,1.0f };
	deadEnemyParticleEmitter.speed = { 2.0f,2.0f,2.0f };

	sleepParticle_ = new ParticleSystem();
	sleepParticle_->Init("project/resources/sleepParticle.png");
	sleepParticle_->UpdateFunc = [this](Particle& particle) { SleepUpdateParticle(particle); };
	sleepParticle_->CustumSpawnFunc = [this]() { return CustomParticle(); };

	sleepParticleEmitter.count = 1;
	sleepParticleEmitter.frequency = 1.0f;
	sleepParticleEmitter.frequencyTime = 0.5f;
	sleepParticleEmitter.particleRadius = { 0.2f,0.2f,0.2f };
	sleepParticleEmitter.world_.transform.translate = {-1.0f,1.0f,12.0f};
	sleepParticleEmitter.color = { 1.0f,1.0f,1.0f };
	sleepParticleEmitter.speed = { 2.0f,2.0f,2.0f };
#pragma endregion パーティクル
#pragma region
	ball = std::make_unique<Ball>();
	std::vector<Model*> ballmodels;
	ballmodels.push_back(Model::CreateModelFromObj("project/resources/Cube", "Cube.obj"));
	ball->Init(ballmodels);
	dummyBall = std::make_unique<DummyBall>();
	dummyBall->Init(ballmodels);
#pragma endregion ボール
#pragma region
	SEthrowBall = Audio::LoadAudioMP3("project/resources/sound/Boss/throwBall.mp3", false);
	SEHitattack = Audio::LoadAudioMP3("project/resources/sound/Boss/attackPlayer.mp3", false);
#pragma endregion
	//ビヘイビアーを初期化
	ChangeState<ESpawn>();
	name = "Boss";
	//初期値を設定
	HP_ = 10;
}
void Boss::Update()
{
	//ダメージを受けた時
	if (isDamege) {
		animationArmLDamage->PlayAnimation();
		animationTime_ += kDeltaTime;
		if (animationTime_ > animationArmLDamage->duration) {
			isDamege = false;
			animationTime_ = kDeltaTime;
			colliders_[Boss::ColliderType::WeekPoint].IsUsing = true;

		}
	}
	if (state_->GetStateType() == BossState::AttackThrowball) {
		easeT = (std::min)(easeT + addEaseT, 1.0f);
	}else {
		easeT = 0.0f;
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
bool Boss::FollowPlayer()
{
	//TODO:命名仮
	Vector3 temp = player_->GetWorld().transform.translate - world_.transform.translate;
	//モデルの中心から手のひらへ
	temp.z += 5.0f;
	temp.y = 0.0f;
	float playerToEnemyLngth = temp.Length();
	temp = temp.Normalize();
	world_.transform.translate += temp * 0.5f;
	//TODO:命名仮
	if (playerToEnemyLngth <= 0.2f) {
		return true;
	}
	return false;
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
		hitCount += 1;
		if (HP_ <= 0) {
			ChangeState<EDead>();
		}
		isDamege = true;
		damegeInterval = 0;
		colliders_[Boss::ColliderType::WeekPoint].IsUsing = false;
	}

	if (collider.GetcollitionAttribute() == Collider::Tag::EnemyBall) {
		if (state_->GetStateType() == BossState::AttackThrowball) {
			ball->Reset(player_->GetWorld().transform.translate);
			if (hitCount < 3) {
				Audio::Stop(SEthrowBall, true, false);
				Audio::Play(SEthrowBall, 0.2f);
			}
			if (easeT == 1.0f) {
				countHitBall += 1;
				easeT = 0.0f;
			}
		}
	}
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
	if (collider.GetcollitionAttribute() == Collider::Tag::Player && state_->GetStateType() == BossState::AttackSlam) {
		colliders_[Boss::ColliderType::Arm].IsUsing = false;
		colliders_[Boss::ColliderType::Hund].IsUsing = false;
		Audio::Play(SEHitattack, 1.0f);
	}
}
#pragma endregion Collider
void Boss::AddImGui()
{
	if (ImGui::Button("AttackMove")) {
		ChangeState<EAttackSlam>();
	}
	ImGui::Text(state_->ShowState().c_str());
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
	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	particle.lifeTime = ParticleSystem::SetParticleLifeTime(randomEngine,1.0f,3.0f);
	particle.transform.scale = sleepParticleEmitter.particleRadius;
	particle.transform.translate = sleepParticleEmitter.world_.transform.translate;

	return particle;
}
void Boss::SleepUpdateParticle(Particle& particle)
{
	particle.transform.translate.y += sleepParticleValue * kDeltaTime;
	if (particle.transform.translate.y <= 10.0f) {
		particle.transform.translate.x += std::sin(std::clamp(particle.transform.translate.y,0.0f,1.0f)) * kDeltaTime * -1.0f;
	}
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = sleepParticleEmitter.particleRadius;
	Vector3 translate = particle.transform.translate + sleepParticleEmitter.world_.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	particle.color.w = alpha;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}