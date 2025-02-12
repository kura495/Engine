#pragma once
#include "State/Play/GamePlayPhase/GamePlayStatePhase.h"
class ClearPhase :GamePlayStatePhase {
public:
	void Init(GamePlayState* PlayState)override;
	void Update(GamePlayState* PlayState)override;
	void Draw(GamePlayState* PlayState)override;
private:

};
