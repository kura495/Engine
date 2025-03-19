#pragma once
#include "State/Play/GamePlayPhase/GamePlayStatePhase.h"
#include "application/Object/UI/Life/Life_UI.h"
class GameOverPhase : public GamePlayStatePhase {
public:
	enum Phase {
		FadeIn,
		ShowLifeUI,
		END,
	};
	void Init(GamePlayState* playState)override;
	void Update(GamePlayState* playState)override;
	void Draw(GamePlayState* playState)override;
private:
	int BGMHundle;
	float audioValue = 0.0f;
	float kMaxaudioValue = 0.04f;
	float audioSubValue = 0.001f;
	//画面遷移演出
	Fade fade;
	//Life_UI
	std::unique_ptr<Life_UI> life_UI;

	uint8_t phase = Phase::FadeIn;
};