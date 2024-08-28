#include"Scenes/State/GameClearState.h"

void GameClearState::Initialize()
{
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
	input->GetJoystickState(joyState);
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		StateNo = 0;
	}
}

void GameClearState::Draw()
{
	texture->Draw(texture_world_);
}

