#pragma once
/*今回ゲームオブジェクト*/
#include "Object/IObject.h"
#include "ParticleSystem/ParticleSystem.h"

class GameCharacter : public IObject {
public:
	void ParticleSpawn(ParticleSystem& particle, Emitter& emitter);
	void ParticleCustumSpawn(ParticleSystem& particle, Emitter& emitter);

};