#pragma once
#include "State/Play/GamePlayPhase/GamePlayStatePhase.h"
class ClearPhase : public GamePlayStatePhase {
public:
	void Init(GamePlayState* playState)override;
	void Update(GamePlayState* playState)override;
	void Draw(GamePlayState* playState)override;
private:

};
