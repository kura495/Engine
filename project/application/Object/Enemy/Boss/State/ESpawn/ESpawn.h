#pragma once
#include "../IBossState.h"
class ESpawn : public IBossState
{
public:
	ESpawn();

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;

private:
	//初期位置
	Vector3 SpawnPosition{ 0.0f,0.5f,20.0f };
	//寝てる演出パーティクル
	std::unique_ptr<ParticleSystem> sleepParticle_;
	Emitter sleepParticleEmitter;
	void SleepUpdateParticle(Particle& particle);
	float sleepParticleValue = 2.0f;

	Particle CustomParticle();
};