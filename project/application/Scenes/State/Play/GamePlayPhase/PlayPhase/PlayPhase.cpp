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
		playState->ChangePhase<GameOverPhase>();
	}
}

void PlayPhase::Draw(GamePlayState* playState)
{
	playState->GetPlayer()->Draw();
}