#pragma once
#include "UFO/state/IUFOState.h"
class StartAnime : public IUFOState
{
public:
	StartAnime() { stateType = UFOState::Normal; }
	void Init(UFO* ufo)override;
	void Update(UFO* ufo)override;
	void Draw(UFO* ufo)override;
private:

};