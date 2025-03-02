#include "ReStertPhase.h"
#include "State/Play/GamePlayState.h"
void ReStertPhase::Init(GamePlayState* playState)
{
	playState;
	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);
	Audio::Play(BGMHundle, audioValue);
	fade.OutInit();
	fadeOutFlag = false;
	playState->ReStert();

}

void ReStertPhase::Update(GamePlayState* playState)
{
	easeT = (std::min)(easeT + kDeltaTime, 1.0f);
	audioValue = (std::min)(audioValue + 0.001f, kMaxaudioValue);
	Audio::Play(BGMHundle, audioValue);

	playState->GetFollowCamera()->Update();
	playState->GetPlayer()->Update();
	playState->GetCollisionManager()->Update();

	if (fade.Out(2.0f)) {
		//playState->GetPlayer()->Update();
		playState->GetEnemyManager()->Update();

		if (playState->GetPlayer()->isReStertFlag) {
			playState->ChangePhase<PlayPhase>();
		}
	}
}

void ReStertPhase::Draw(GamePlayState* playState)
{
	playState->GetPlayer()->Draw();
	fade.Draw();
}
