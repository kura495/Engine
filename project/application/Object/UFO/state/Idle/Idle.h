#pragma once
#include "UFO/state/IUFOState.h"
class Idle : public IUFOState
{
public:
	Idle() { stateType = UFOState::Idle; }
	void Init(UFO* ufo)override;
	void Update(UFO* ufo)override;
	void Draw(UFO* ufo)override;
private:

};
