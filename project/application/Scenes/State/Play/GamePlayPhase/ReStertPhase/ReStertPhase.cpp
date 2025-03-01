#include "ReStertPhase.h"
#include "State/Play/GamePlayState.h"
void ReStertPhase::Init(GamePlayState* playState)
{
	playState;
	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);
	Audio::Play(BGMHundle, audioValue);
	fade.OutInit();
	fadeOutFlag = false;
}

void ReStertPhase::Update(GamePlayState* playState)
{
	easeT = (std::min)(easeT + kDeltaTime, 1.0f);
	audioValue = (std::min)(audioValue + 0.001f, kMaxaudioValue);
	Audio::Play(BGMHundle, audioValue);
	if (easeT == 1.0f && fadeOutFlag == false) {
		playState->ReStert();
		fadeOutFlag = true;
	}
	if (fadeOutFlag) {
		if (fade.Out(5.0f)) {
			playState->ChangePhase<PlayPhase>();
		}
	}

}

void ReStertPhase::Draw(GamePlayState* playState)
{
	playState;
	fade.Draw();
}
