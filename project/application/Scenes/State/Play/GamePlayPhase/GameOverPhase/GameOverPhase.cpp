#include "GameOverPhase.h"
#include "State/Play/GamePlayState.h"
void GameOverPhase::Init(GamePlayState* playState)
{
	playState;
	fade.InInit("project/resources/BlackTexture.png");
	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);

	life_UI = std::make_unique<Life_UI>();
	life_UI->Init(playState->GetPlayer()->GetHP());
}

void GameOverPhase::Update(GamePlayState* playState)
{
	playState->GetPlayer()->Update();
	Audio::Play(BGMHundle, (std::max)(audioValue - audioSubValue, 0.0f));
	if (phase == Phase::FadeIn) {
		if (fade.In(0.4f)) {
			phase = Phase::ShowLifeUI;
		}
	}
	if (phase == Phase::ShowLifeUI) {
		if (life_UI->Animation()) {
			playState->StateNo = GameStateNo::GAMEOVER;
		}
	}
}

void GameOverPhase::Draw(GamePlayState* playState)
{
	playState->GetPlayer()->Draw();
	fade.Draw();
	if (phase == Phase::ShowLifeUI) {
		life_UI->Draw();
	}
}