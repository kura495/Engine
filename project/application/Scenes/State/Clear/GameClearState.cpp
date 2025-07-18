#include"Scenes/State/Clear/GameClearState.h"

void GameClearState::Init()
{
	StateNo = GameStateNo::CLEAR;

	Editer::GetInstance()->IsEnable(false);
	textureManager_ = TextureManager::GetInstance();
	bg_world_.Init();
	
	background_ = std::make_unique<Sprite>();
	background_->TextureHandle = textureManager_->LoadTexture("project/resources/Clear.png");
	background_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });


	ui_world_.Init();

	ui_ = std::make_unique<Sprite>();
	ui_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/AButton_ui.png");
	ui_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	
	fade.InInit("project/resources/BlackTexture.png");
}

void GameClearState::Update()
{
	ui_move_Value_ -= ui_move_add_Value_;

	if (ui_move_Value_ >= ui_move_MAX_) {
		ui_move_add_Value_ *= -1;
	}
	else if (ui_move_Value_ <= ui_move_MIN_) {
		ui_move_add_Value_ *= -1;
	}

	ui_world_.transform.translate.y = ui_move_Value_;
	ui_world_.Update();

	time++;
	if (IsCanPush == false && fade.Out(1.0f) == false) {
		return;
	}


	if (time > 60 && Input::GetPadPrecede(XINPUT_GAMEPAD_A, 10)) {
		IsCanPush = true;
	}

	if (IsCanPush) {
		if (fade.In(1.0f)) {
			StateNo = GameStateNo::TITLE;
			IsCanPush = false;
		}
	}
}

void GameClearState::Draw()
{

	background_->RendererDraw(bg_world_);
	ui_->RendererDraw(ui_world_);

	fade.Draw();
}

