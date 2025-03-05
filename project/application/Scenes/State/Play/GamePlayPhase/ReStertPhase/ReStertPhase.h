#pragma once
#include "State/Play/GamePlayPhase/GamePlayStatePhase.h"

class ReStertPhase : public GamePlayStatePhase{
public:
	enum Phase {
		FadeIn,
		ReStertFanc,
		FadeOut,
		END,
	};
	void Init(GamePlayState* playState)override;
	void Update(GamePlayState* playState)override;
	void Draw(GamePlayState* playState)override;
private:
	int BGMHundle;
	float audioValue = 0.0f;
	float kMaxaudioValue = 0.04f;
	//画面遷移演出
	Fade fade;
	//現在のTの値
	float easeT = 0.0f;

	uint8_t phase = Phase::FadeIn;
};

