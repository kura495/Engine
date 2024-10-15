#include"Scenes/State/GameOver/GameOverState.h"

void GameOverState::Initialize()
{
	StateNo = GameStateNo::GAMEOVER;

	Editer::GetInstance()->IsEnable(false);
	
}

void GameOverState::Update()
{

}

void GameOverState::Draw()
{

}

