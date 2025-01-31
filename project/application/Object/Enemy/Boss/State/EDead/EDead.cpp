#include "EDead.h"
#include "../../Boss.h"
void EDead::Init(Boss* Boss)
{
	Boss->deadEnemyParticleEmitter.world_.transform.translate = Boss->GetWorld().transform.translate;
	easeT = 0.0f;
}

void EDead::Update(Boss* Boss)
{
	easeT = (std::min)(easeT + 0.1f, 1.0f);

	Boss->Getmodels()[0]->color_.w = (std::max)(Boss->Getmodels()[0]->color_.w - 0.01f, 0.0f);
	if (Boss->Getmodels()[0]->color_.w == 0.0f) {
		Boss->IsAlive = false;
	}
	//パーティクル生成
	ParticleSystem::ParticleSpawn(*Boss->deadParticle_, Boss->deadEnemyParticleEmitter);
	//パーティクル更新
	Boss->deadParticle_->Update();
}

void EDead::Draw(Boss* Boss)
{
	Boss->Getmodels()[0]->RendererSkinDraw(Boss->GetWorld(), Boss->GetAnime()->GetSkinCluster());
	Boss->deadParticle_->RendererDraw();
}
std::string EDead::ShowState()
{
	return "EDead";
}