#include"Scenes/State/Title/GameTitleState.h"

void GameTitleState::Initialize()
{
	StateNo = GameStateNo::TITLE;

	Editer::GetInstance()->SetViewProjection(&Renderer::viewProjection);
	Editer::GetInstance()->IsEnable(false);
}

void GameTitleState::Update()
{
	
}

void GameTitleState::Draw()
{
	
}