#pragma once
#include "UFO/state/IUFOState.h"
class Boost : public IUFOState
{
public:
	Boost() { stateType = UFOState::Boost; }
	void Init(UFO* ufo)override;
	void Update(UFO* ufo)override;
	void Draw(UFO* ufo)override;
private:
	//中心
	Vector3 homePosition = { 0.0f,0.0f,0.0f };
};
