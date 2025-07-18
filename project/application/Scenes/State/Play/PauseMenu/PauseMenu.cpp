#include "PauseMenu.h"

void PauseMenu::Init()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	sprite_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/BlackTexture.png");
	sprite_->SetColor({ 1.0f,1.0f,1.0f,0.7f });

	tutorialSprite_ = std::make_unique<Sprite>();
	tutorialSprite_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	tutorialSprite_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/control_Tuto.png");

	UI_manual = std::make_unique<Sprite>();
	UI_manual->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	UI_manual->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/tuto_UI.png");

	world_.Init();
}
bool PauseMenu::Update()
{
	//ボタンを押すとtrueとfalseが切り替わる
	if (IsPause) {
		//ポーズメニューがオンの時の処理
		if (Input::IsTriggerPad(XINPUT_GAMEPAD_START)) {
			IsPause = false;
		}
		return true;
	}
	else if (IsPause == false) {
		//ポーズメニューがオフの時の処理
		if (Input::IsTriggerPad(XINPUT_GAMEPAD_START)) {
			IsPause = true;
		}
		return false;
	}
	return false;
}
void PauseMenu::Draw()
{
	if (IsPause) {
		sprite_->RendererDraw(world_);
		tutorialSprite_->RendererDraw(world_);
	}

	UI_manual->RendererDraw(world_);

}