#include "PlayPhase.h"
#include "State/Play/GamePlayState.h"
void PlayPhase::Init(GamePlayState* playState)
{
	playState;
}

void PlayPhase::Update(GamePlayState* playState)
{
	playState->GetPlayer()->Update();
	playState->GetCollisionManager()->Update();
	playState->GetEnemyManager()->Update();
	playState->GetFollowCamera()->Update();

	if (playState->GetEnemyManager()->GetisClear()) {
		playState->ChangePhase<ClearPhase>();
	}
	if (playState->GetPlayer()->GetisDead()) {
		if (playState->GetPlayer()->GetHP() <= 0) {
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