#pragma once
#include "State/Play/GamePlayPhase/GamePlayStatePhase.h"
class ReStertPhase : public GamePlayStatePhase{
public:
	void Init(GamePlayState* playState)override;
	void Update(GamePlayState* playState)override;
	void Draw(GamePlayState* playState)override;
private:
	int BGMHundle;
	float audioValue = 0.0f;
	float kMaxaudioValue = 0.04f;
};

