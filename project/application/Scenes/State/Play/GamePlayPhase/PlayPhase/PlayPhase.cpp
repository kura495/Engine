#include "PlayPhase.h"
#include "State/Play/GamePlayState.h"
void PlayPhase::Init(GamePlayState* playState)
{
	playState->GetPlayer()->ChangeState<PJump>();
}

void PlayPhase::Update(GamePlayState* playState)
{
	playState->GetPlayer()->Update();
	playState->enemyManager->Update();
	playState->followCamera->Update();

	if (playState->enemyManager->GetisClear()) {
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