#include"Scenes/State/Clear/GameClearState.h"

void GameClearState::Init()
{
	StateNo = GameStateNo::CLEAR;

	Editer::GetInstance()->IsEnable(false);
	textureManager_ = TextureManager::GetInstance();
	texture_world_.Init();
	
	texture = std::make_unique<Sprite>();
	texture->TextureHandle = textureManager_->LoadTexture("project/resources/Clear.png");
	texture->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });

	
	fade.OutInit();
}

void GameClearState::Update()
{
	time++;
	if (IsCanPush == false && fade.Out() == false) {
		return;
	}


	if (time > 60 && Input::GetPadPrecede(XINPUT_GAMEPAD_A, 10)) {
		IsCanPush = true;
	}

	if (IsCanPush) {
		if (fade.In()) {
			StateNo = 1;
			IsCanPush = false;
		}
	}
}

void GameClearState::Draw()
{

	texture->RendererDraw(texture_world_);
	fade.Draw();
}

