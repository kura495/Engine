#include "EDead.h"
#include "../../Boss.h"
void EDead::Init(Boss* boss)
{
	boss->deadEnemyParticleEmitter.world_.transform.translate = boss->GetWorld().transform.translate;
	easeT = 0.0f;
}

void EDead::Update(Boss* boss)
{
	easeT = (std::min)(easeT + 0.1f, 1.0f);

	boss->Getmodels()[0]->color_.w = (std::max)(boss->Getmodels()[0]->color_.w - 0.01f, 0.0f);
	if (boss->Getmodels()[0]->color_.w == 0.0f) {
		boss->IsAlive = false;
	}
	//パーティクル生成
	ParticleSystem::ParticleSpawn(*boss->deadParticle_, boss->deadEnemyParticleEmitter);
	//パーティクル更新
	boss->deadParticle_->Update();
}

void EDead::Draw(Boss* boss)
{
	boss->Getmodels()[0]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
	boss->deadParticle_->RendererDraw();
}
std::string EDead::ShowState()
{
	return "EDead";
}