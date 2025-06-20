#pragma once
#include "State/Play/GamePlayPhase/GamePlayStatePhase.h"
#include "application/Object/UI/Life/Life_UI.h"

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
	//Life_UI
	std::unique_ptr<Life_UI> life_UI;
	//画面遷移演出
	Fade fade;
	//現在のTの値
	float easeT = 0.0f;

	uint8_t phase = Phase::ReStertFanc;
};

