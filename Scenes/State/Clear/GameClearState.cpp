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

	fade = Fade::GetInstance();
	fade->OutInit();
}

void GameClearState::Update()
{
	time++;
	if (IsCanPush == false &&fade->Out() == false) {
		return;
	}


	if (time > 60 && input->GetPadPrecede(XINPUT_GAMEPAD_A, 10)) {
		IsCanPush = true;
	}

	if (IsCanPush) {
		if (fade->In()) {
			StateNo = 1;
			IsCanPush = false;
		}
	}
}

void GameClearState::Draw()
{

	texture->RendererDraw(texture_world_);
	fade->Draw();
}

