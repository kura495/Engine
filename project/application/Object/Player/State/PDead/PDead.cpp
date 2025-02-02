#include "PDead.h"
#include "../../Player.h"
void PDead::Init(Player* p)
{
	p->deadParticleEmitter.world_.transform.translate = p->GetWorld().transform.translate;
	p->deadParticleEmitter.world_.transform.translate.y += 1.0f;
	p->deadParticleEmitter.frequencyTime = p->deadParticleEmitter.frequency;
	animationTime_ = kDeltaTime;
}

void PDead::Update(Player* p)
{
	//p->isOnFloorFlag = true;
	//死亡アニメーション更新

	animationTime_ += kDeltaTime;
	p->deadAnimation->PlayAnimation(false);
	if (animationTime_ >= p->deadAnimation->duration) {
		p->isDamege = false;
		animationTime_ = 0.0f;
		isDeadModelDraw = false;
		p->isDead = true;
	}
	if (isDeadModelDraw == false) {
		//パーティクル生成
		ParticleSystem::ParticleSpawn(*p->deadParticle_, p->deadParticleEmitter);
		p->deadParticle_->Update();
	}
}

void PDead::Draw(Player* p)
{
	if (isDeadModelDraw) {
		p->Getmodels()[0]->RendererSkinDraw(p->GetWorld(), p->deadAnimation->GetSkinCluster());
	}
	else {
		p->deadParticle_->RendererDraw();
	}
}

std::string PDead::ShowState()
{
	return "PDead";
}
