#include "EDead.h"
#include "../../Boss.h"
EDead::EDead()
{
	stateType = BossState::Dead;
	deadParticle_ = std::make_unique<ParticleSystem>();
	deadParticle_->Init("project/resources/circle2.dds");
	deadParticle_->UpdateFunc = [this](Particle& particle) { UpdateParticle(particle); };

}
void EDead::Init(Boss* boss)
{
	deadEnemyParticleEmitter.count = 50;
	deadEnemyParticleEmitter.frequency = 0.1f;
	deadEnemyParticleEmitter.particleRadius = { 1.0f,1.0f,1.0f };
	deadEnemyParticleEmitter.color = { 1.0f,1.0f,1.0f };
	deadEnemyParticleEmitter.speed = { 2.0f,2.0f,2.0f };
	deadEnemyParticleEmitter.world_.transform.translate = boss->GetWorld().transform.translate;
	easeT = 0.0f;
}

void EDead::Update(Boss* boss)
{
	easeT = (std::min)(easeT + 0.1f, 1.0f);

	boss->Getmodels()[Boss::BossModel::MainBody]->color_.w = (std::max)(boss->Getmodels()[Boss::BossModel::MainBody]->color_.w - 0.01f, 0.0f);
	if (boss->Getmodels()[Boss::BossModel::MainBody]->color_.w == 0.0f) {
		boss->IsAlive = false;
	}
	//パーティクル生成
	ParticleSystem::ParticleSpawn(*deadParticle_, deadEnemyParticleEmitter);
	//パーティクル更新
	deadParticle_->Update();
}

void EDead::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
	deadParticle_->RendererDraw();
}
std::string EDead::ShowState()
{
	return "EDead";
}

void EDead::UpdateParticle(Particle& particle)
{
	Vector3 velcity = particle.velocity * kDeltaTime;
	particle.transform.translate += velcity * deadEnemyParticleEmitter.speed;
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = deadEnemyParticleEmitter.particleRadius;
	Vector3 translate = particle.transform.translate + deadEnemyParticleEmitter.world_.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	deadEnemyParticleEmitter.color.w = alpha;
	particle.color = deadEnemyParticleEmitter.color;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}
