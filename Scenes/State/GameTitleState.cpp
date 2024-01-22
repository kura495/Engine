#include"Scenes/State/GameTitleState.h"

void GameTitleState::Initialize()
{
	input = Input::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	textureHundle = textureManager_->LoadTexture("resources/title.png");
	texture_world_.Initialize();

	texture = std::make_unique<Sprite>();
	texture->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });

	time = 0;
	IsCanPush = false;
}

void GameTitleState::Update()
{
	time++;
	if (time > 60) {
		IsCanPush = true;
	}

	input->GetJoystickState(joyState);
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		if (IsCanPush) {
			StateNo = 1;
		}
	}
}

void GameTitleState::Draw()
{
	texture->Draw(texture_world_, textureHundle);
}
