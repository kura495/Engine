#pragma once
#include "../IBossState.h"
class EDead : public IBossState
{
public:
	EDead();

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
private:
	//死亡パーティクル
	std::unique_ptr<ParticleSystem> deadParticle_;
	Emitter deadEnemyParticleEmitter;
	void UpdateParticle(Particle& particle);
};
