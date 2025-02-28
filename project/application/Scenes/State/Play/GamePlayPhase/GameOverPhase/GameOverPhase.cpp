#include "GameOverPhase.h"
#include "State/Play/GamePlayState.h"
void GameOverPhase::Init(GamePlayState* playState)
{
	playState;
	fade.InInit();
	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);
}

void GameOverPhase::Update(GamePlayState* playState)
{
	playState->GetPlayer()->Update();
	Audio::Play(BGMHundle, (std::max)(audioValue - audioSubValue, 0.0f));
	if (fade.In(0.4f)) {
		if (playState->GetPlayer()->GetHP() <= 0) {
			playState->StateNo = GameStateNo::GAMEOVER;
		}
		else {
			playState->ChangePhase<ReStertPhase>();
		}
	}
}

void GameOverPhase::Draw(GamePlayState* playState)
{
	playState->GetPlayer()->Draw();
	fade.Draw();
}