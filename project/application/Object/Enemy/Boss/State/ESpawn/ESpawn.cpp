#include "ESpawn.h"
#include "../../Boss.h"
void ESpawn::Init(Boss* boss)
{
	//当たり判定を通常に変更
	boss->SetColliderAttribute(Boss::ColliderType::Arm, Collider::Tag::Enemy);
	boss->SetColliderAttribute(Boss::ColliderType::Hund, Collider::Tag::Enemy);
}

void ESpawn::Update(Boss* boss)
{
	//パーティクル生成
	ParticleSystem::ParticleCustumSpawn(*boss->sleepParticle_, boss->sleepParticleEmitter);
	//パーティクル更新
	boss->sleepParticle_->Update();
	if (boss->GetHP() <= 9) {
		boss->ChangeState<EReturnPosition>();
	}
}

void ESpawn::Draw(Boss* boss)
{
	boss->Getmodels()[0]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
	boss->sleepParticle_->RendererDraw();
}
std::string ESpawn::ShowState()
{
	return "ESpawn";
}