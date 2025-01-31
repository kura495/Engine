#include "ESpawn.h"
#include "../../Boss.h"
void ESpawn::Init(Boss* Boss)
{
	//当たり判定を通常に変更
	Boss->SetColliderAttribute(ColliderNumber::Arm, ColliderTag::Enemy);
	Boss->SetColliderAttribute(ColliderNumber::Hund, ColliderTag::Enemy);
}

void ESpawn::Update(Boss* Boss)
{
	//パーティクル生成
	ParticleSystem::ParticleCustumSpawn(*Boss->sleepParticle_, Boss->sleepParticleEmitter);
	//パーティクル更新
	Boss->sleepParticle_->Update();
	if (Boss->GetHP() <= 9) {
		Boss->ChangeState<EReturnPosition>();
	}
}

void ESpawn::Draw(Boss* Boss)
{
	Boss->Getmodels()[0]->RendererSkinDraw(Boss->GetWorld(), Boss->GetAnime()->GetSkinCluster());
	Boss->sleepParticle_->RendererDraw();
}
std::string ESpawn::ShowState()
{
	return "ESpawn";
}