#include "PlayPhase.h"
#include "State/Play/GamePlayState.h"
float PlayPhase::HitStopCount = 0.0f;
float PlayPhase::TargetTime = 0.0f;
void PlayPhase::Init(GamePlayState* playState)
{
	playState->GetPlayer()->ChangeState<PJump>();
}

void PlayPhase::Update(GamePlayState* playState)
{
	if (HitStopUpdate()) {
		return;
	}

	playState->GetPlayer()->Update();
	playState->GetEnemyManager()->Update();
	playState->GetFollowCamera()->Update();
	playState->GetCollisionManager()->Update();

	if (playState->GetEnemyManager()->GetisClear()) {
		playState->ChangePhase<ClearPhase>();
	}
	if (playState->GetPlayer()->GetisDead()) {
		if (playState->GetPlayer()->GetHP() == 0) {
			playState->ChangePhase<GameOverPhase>();
		}
		else {
			playState->ChangePhase<ReStertPhase>();
		}
	}

}

void PlayPhase::Draw(GamePlayState* playState)
{
	playState->GetPlayer()->Draw();
}

void PlayPhase::HitStop(float second)
{
	HitStopCount = second;
	TargetTime = 0.0f;
}

bool PlayPhase::HitStopUpdate()
{
	TargetTime += kDeltaTime;
	if (TargetTime < HitStopCount) {
		return true;
	}
	return false;
}
