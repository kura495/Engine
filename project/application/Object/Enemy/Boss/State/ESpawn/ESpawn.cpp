#include "ESpawn.h"
#include "../../Boss.h"
ESpawn::ESpawn()
{
	stateType = BossState::Spawn;
	sleepParticle_ = std::make_unique<ParticleSystem>();
	sleepParticle_->Init("project/resources/sleepParticle.png");
	sleepParticle_->UpdateFunc = [this](Particle& particle) { SleepUpdateParticle(particle); };
	sleepParticle_->CustumSpawnFunc = [this]() { return CustomParticle(); };
	//攻撃の選択肢を初期化する
	isAttackSelect = AttackState::RocketPunch;
}
void ESpawn::Init(Boss* boss)
{
	//当たり判定を通常に変更
	boss->SetColliderAttribute(Boss::ColliderType::AttackArm, Collider::Tag::Enemy);
	boss->SetColliderAttribute(Boss::ColliderType::AttackHund, Collider::Tag::Enemy);

	boss->GetWorld().transform.translate = SpawnPosition;
	boss->GetWorld().Update();

	sleepParticleEmitter.count = 1;
	sleepParticleEmitter.frequency = 1.0f;
	sleepParticleEmitter.frequencyTime = 0.5f;
	sleepParticleEmitter.particleRadius = { 0.2f,0.2f,0.2f };
	sleepParticleEmitter.world_.transform.translate = { -1.0f,1.0f,12.0f };
	sleepParticleEmitter.color = { 1.0f,1.0f,1.0f };
	sleepParticleEmitter.speed = { 2.0f,2.0f,2.0f };
}

void ESpawn::Update(Boss* boss)
{
	//パーティクル生成
	ParticleSystem::ParticleCustumSpawn(*sleepParticle_,sleepParticleEmitter);
	//パーティクル更新
	sleepParticle_->Update();
	//カメラの揺れをオフに
	boss->getCamera()->isShake = false;
	boss->getCamera()->SetFOV(45.0f);


	boss->ChangeState<EReturnPosition>();

}

void ESpawn::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
	sleepParticle_->RendererDraw();
}
std::string ESpawn::ShowState()
{
	return "ESpawn";
}

Particle ESpawn::CustomParticle()
{
	Particle particle{};
	particle.color = { 1.0f,1.0f,1.0f };

	particle.lifeTime = random::Generate<float>(1.0f, 3.0f);
	particle.transform.scale = sleepParticleEmitter.particleRadius;
	particle.transform.translate = sleepParticleEmitter.world_.transform.translate;

	return particle;
}
void ESpawn::SleepUpdateParticle(Particle& particle)
{
	particle.transform.translate.y += sleepParticleValue * kDeltaTime;
	if (particle.transform.translate.y <= 10.0f) {
		particle.transform.translate.x += std::sin(std::clamp(particle.transform.translate.y, 0.0f, 1.0f)) * kDeltaTime * -1.0f;
	}
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = sleepParticleEmitter.particleRadius;
	Vector3 translate = particle.transform.translate + sleepParticleEmitter.world_.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	particle.color.w = alpha;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}
