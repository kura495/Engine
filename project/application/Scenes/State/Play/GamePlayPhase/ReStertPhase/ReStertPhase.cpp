#include "ReStertPhase.h"
#include "State/Play/GamePlayState.h"
void ReStertPhase::Init(GamePlayState* playState)
{
	playState;
	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);
	Audio::Play(BGMHundle, audioValue);
	fade.InInit();

	phase = Phase::FadeIn;
}

void ReStertPhase::Update(GamePlayState* playState)
{
	easeT = (std::min)(easeT + kDeltaTime, 1.0f);
	audioValue = (std::min)(audioValue + 0.001f, kMaxaudioValue);
	Audio::Play(BGMHundle, audioValue);

	if (phase == Phase::FadeIn) {
		if (fade.In(1.0f)) {
			phase = Phase::ReStertFanc;
			easeT = 0.0f;
		}
	}
	else if (phase == Phase::ReStertFanc) {
		if (easeT == 1.0f) {
			playState->ReStert();
			phase = Phase::FadeOut;
			fade.OutInit();
		}
	}
	else if (phase == Phase::FadeOut) {

		if (fade.Out(3.0f)) {
			playState->ChangePhase<PlayPhase>();
		}
	}
}

void ReStertPhase::Draw(GamePlayState* playState)
{
	playState->GetPlayer()->Draw();
	fade.Draw();
}
