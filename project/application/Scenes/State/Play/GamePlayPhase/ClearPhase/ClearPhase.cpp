#include "ClearPhase.h"
#include "State/Play/GamePlayState.h"
void ClearPhase::Init(GamePlayState* playState)
{
	playState;
	fade.InInit();
	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);
}

void ClearPhase::Update(GamePlayState* playState)
{
	audioValue = (std::max)(audioValue - 0.001f, 0.0f);
	Audio::Play(BGMHundle, audioValue);

	if (fade.In()) {
		playState->StateNo = 2;
	}
}

void ClearPhase::Draw(GamePlayState* playState)
{
	playState->player_->Draw();
	fade.Draw();
}