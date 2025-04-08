#pragma once
#include "ParticleSystem/ParticleSystem.h"
class UFO;	
enum class UFOState {
	Idle,
	Normal,
	Boost,
	StartAnime,
};
class IUFOState {
public:
	IUFOState() {};
	virtual ~IUFOState() {};


	virtual void Init(UFO* ufo) =0;
	virtual void Update(UFO* ufo) =0;
	virtual void Draw(UFO* ufo) =0;
	UFOState GetStateType() { return stateType; };
protected:
	UFOState stateType;
};