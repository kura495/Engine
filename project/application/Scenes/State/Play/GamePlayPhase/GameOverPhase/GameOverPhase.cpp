#include "GameOverPhase.h"
#include "State/Play/GamePlayState.h"
void GameOverPhase::Init(GamePlayState* playState)
{
	playState;
	fade.Init();
	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);
}

void GameOverPhase::Update(GamePlayState* playState)
{
	playState->player_->Update();
	audioValue = (std::max)(audioValue - 0.001f, 0.0f);
	Audio::Play(BGMHundle, audioValue);
	if (fade.In(1.0f)) {
		playState->StateNo = 3;
	}
}

void GameOverPhase::Draw(GamePlayState* playState)
{
	playState->player_->Draw();
	fade.Draw();
}