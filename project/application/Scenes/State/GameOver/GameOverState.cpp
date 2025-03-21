#include"Scenes/State/GameOver/GameOverState.h"

void GameOverState::Init()
{
	StateNo = GameStateNo::GAMEOVER;

	Editer::GetInstance()->IsEnable(false);

	textureManager_ = TextureManager::GetInstance();
	texture_world_.Init();

	texture = std::make_unique<Sprite>();
	texture->TextureHandle = textureManager_->LoadTexture("project/resources/GameOver.png");
	texture->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
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

