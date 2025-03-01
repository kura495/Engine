#include "ReStertPhase.h"
#include "State/Play/GamePlayState.h"
void ReStertPhase::Init(GamePlayState* playState)
{
	playState;
	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);
	Audio::Play(BGMHundle, audioValue);
	fade.OutInit();
}

void ReStertPhase::Update(GamePlayState* playState)
{
	audioValue = (std::min)(audioValue + 0.001f, kMaxaudioValue);
	Audio::Play(BGMHundle, audioValue);
	if (audioValue == kMaxaudioValue) {
		playState->ReStert();
		playState->ChangePhase<PlayPhase>();
	}
}

void ReStertPhase::Draw(GamePlayState* playState)
{
	playState;
	fade.Draw();
}
