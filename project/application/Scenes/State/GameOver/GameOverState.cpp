#include"Scenes/State/GameOver/GameOverState.h"

void GameOverState::Init()
{
	StateNo = GameStateNo::GAMEOVER;

	Editer::GetInstance()->IsEnable(false);

}

void GameOverState::Update()
{
	time++;
	if (time > 60) {
		IsCanPush = true;
	}
	if (Input::GetPadPrecede(XINPUT_GAMEPAD_A, 10)) {
		if (IsCanPush) {
			StateNo = GameStateNo::PLAY;
		}
	}
}

void GameOverState::Draw()
{
	texture->RendererDraw(texture_world_);
}

