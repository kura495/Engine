#include "PlayPhase.h"
#include "State/Play/GamePlayState.h"
void PlayPhase::Init(GamePlayState* playState)
{
	playState->IsTitleToGameFlag = false;
	playState->player_->ChangeState<PJump>();
}

void PlayPhase::Update(GamePlayState* playState)
{
	playState->player_->Update();
	playState->enemyManager->Update();
	playState->collisionManager->Update();
	playState->followCamera->Update();
}

void PlayPhase::Draw(GamePlayState* playState)
{
	playState->player_->Draw();
}