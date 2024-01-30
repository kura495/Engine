#include"Scenes/State/GameTitleState.h"

void GameTitleState::Initialize()
{
	Editer::GetInstance()->IsEnable(false);
	input = Input::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	time = 0;
	IsCanPush = false;
}

void GameTitleState::Update()
{
	time++;
	if (time > 60) {
		StateNo = 1;
	}

}

void GameTitleState::Draw()
{

}
