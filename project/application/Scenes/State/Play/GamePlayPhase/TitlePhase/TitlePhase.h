#pragma once
#include "State/Play/GamePlayPhase/GamePlayStatePhase.h"
class TitlePhase : public GamePlayStatePhase {
public:
	void Init(GamePlayState* playState)override;
	void Update(GamePlayState* playState)override;
	void Draw(GamePlayState* playState)override;
private:
	int BGMHundle;
	float audioValue = 0.0f;
	float kMaxaudioValue = 0.04f;
	//ボタンを押したときに加算する。一定値を超えたらプレイフェーズに移行する
	float stertCount = 0.0f;
	const float subStertCount = 0.005f;
	//画面遷移演出
	Fade fade;
};