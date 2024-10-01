#include"Scenes/State/Clear/GameClearState.h"

void GameClearState::Initialize()
{
	StateNo = GameStateNo::CLEAR;

	Editer::GetInstance()->IsEnable(false);
	input = Input::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	texture_world_.Initialize();
	
	texture = std::make_unique<Sprite>();
	texture->TextureHandle = textureManager_->LoadTexture("resources/Clear.png");
	texture->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });

}

void GameClearState::Update()
{
	time++;
	if (time > 60) {
		IsCanPush = true;
	}
	if (input->GetPadPrecede(XINPUT_GAMEPAD_A, 10)) {
		if (IsCanPush) {
			StateNo = 0;
		}
	}
}

void GameClearState::Draw()
{
	texture->RendererDraw(texture_world_);
}

