#include "ClearPhase.h"
#include "State/Play/GamePlayState.h"
void ClearPhase::Init(GamePlayState* playState)
{
	playState;
	fade.InInit("project/resources/BlackTexture.png");
	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);
}

void ClearPhase::Update(GamePlayState* playState)
{
	audioValue = (std::max)(audioValue - 0.001f, 0.0f);
	Audio::Play(BGMHundle, audioValue);

	if (fade.In(1.0f)) {
		playState->StateNo = GameStateNo::CLEAR;
	}
}

void ClearPhase::Draw(GamePlayState* playState)
{
	playState->GetPlayer()->Draw();
	fade.Draw();
}