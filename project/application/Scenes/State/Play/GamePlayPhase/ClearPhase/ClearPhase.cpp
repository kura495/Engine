#include "ClearPhase.h"
#include "State/Play/GamePlayState.h"
void ClearPhase::Init(GamePlayState* playState)
{
	playState;
}

void ClearPhase::Update(GamePlayState* playState)
{
	playState->audioValue = (std::max)(playState->audioValue - 0.001f, 0.0f);
	Audio::Play(playState->BGMHundle, playState->audioValue);

	if (playState->fade.In()) {
		playState->StateNo = 2;
	}
}

void ClearPhase::Draw(GamePlayState* playState)
{
	playState->player_->Draw();
	playState->fade.Draw();
}