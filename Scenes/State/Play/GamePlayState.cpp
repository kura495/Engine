#include "Scenes/State/Play/GamePlayState.h"

void GamePlayState::Initialize()
{
	StateNo = GameStateNo::PLAY;

	
}

void GamePlayState::Update()
{
	BehaviorUpdate();

}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

}
#pragma region
void GamePlayState::BehaviorUpdate()
{

}