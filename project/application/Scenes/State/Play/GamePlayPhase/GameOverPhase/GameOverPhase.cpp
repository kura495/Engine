#include "GameOverPhase.h"
#include "State/Play/GamePlayState.h"
void GameOverPhase::Init(GamePlayState* playState)
{
	playState;
}

void GameOverPhase::Update(GamePlayState* playState)
{
	playState->player_->Update();
	playState->audioValue = (std::max)(playState->audioValue - 0.001f, 0.0f);
	Audio::Play(playState->BGMHundle, playState->audioValue);
	if (playState->fade.In()) {
		playState->StateNo = 3;
	}
}

void GameOverPhase::Draw(GamePlayState* playState)
{
	playState->player_->Draw();
	playState->fade.Draw();
}