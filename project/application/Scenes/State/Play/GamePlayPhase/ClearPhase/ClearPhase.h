#pragma once
#include "State/Play/GamePlayPhase/GamePlayStatePhase.h"
class ClearPhase : public GamePlayStatePhase {
public:
	void Init(GamePlayState* playState)override;
	void Update(GamePlayState* playState)override;
	void Draw(GamePlayState* playState)override;
private:
	int BGMHundle;
	float audioValue = 0.0f;
	float kMaxaudioValue = 0.04f;
	//画面遷移演出
	Fade fade;
};
