#include"Scenes/State/GameOver/GameOverState.h"

void GameOverState::Init()
{
	StateNo = GameStateNo::GAMEOVER;

	Editer::GetInstance()->IsEnable(false);

	ui_move_Value_ = 0.0f;

	bg_world_.Init();

	background_ = std::make_unique<Sprite>();
	background_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/GameOver.png");
	background_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });

	ui_world_.Init();

	ui_ = std::make_unique<Sprite>();
	ui_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/AButton_ui.png");
	ui_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
}

void GameOverState::Update()
{
	ui_move_Value_ -= ui_move_add_Value_;

	if (ui_move_Value_ >= ui_move_MAX_) {
		ui_move_add_Value_ *= -1;
	}
	else if (ui_move_Value_ <= ui_move_MIN_) {
		ui_move_add_Value_ *= -1;
	}

	ImGui::Begin("GameOver_UI");
	ImGui::DragFloat("ui_move_add_Value_",&ui_move_add_Value_);
	ImGui::DragFloat("ui_move_MAX_",&ui_move_MAX_);
	ImGui::DragFloat("ui_move_MIN_",&ui_move_MIN_);
	ImGui::End();

	ui_world_.transform.translate.y = ui_move_Value_;
	ui_world_.Update();

	time++;
	if (time > 60) {
		IsCanPush = true;
	}
	if (Input::GetPadPrecede(XINPUT_GAMEPAD_A, 10)) {
		if (IsCanPush) {
			StateNo = GameStateNo::TITLE;
		}
	}
}

void GameOverState::Draw()
{
	background_->RendererDraw(bg_world_);
	ui_->RendererDraw(ui_world_);
}

