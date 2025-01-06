#include "GameCharacter.h"

void GameCharacter::ParticleSpawn(ParticleSystem& particle, Emitter& emitter)
{
	emitter.frequencyTime+= kDeltaTime;
	if (emitter.frequency <= emitter.frequencyTime) {
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());

		particle.SpawnParticle(emitter, randomEngine);

		emitter.frequencyTime -= emitter.frequency;
	}
}

void GameCharacter::ParticleCustumSpawn(ParticleSystem& particle, Emitter& emitter)
{
	emitter.frequencyTime += kDeltaTime;
	if (emitter.frequency <= emitter.frequencyTime) {
		particle.CustumSpawnParticle(emitter);

		emitter.frequencyTime -= emitter.frequency;
	}
}
