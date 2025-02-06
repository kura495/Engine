#include "PauseMenu.h"

void PauseMenu::Init()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	sprite_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/BlackTexture.png");
	sprite_->SetColor({ 1.0f,1.0f,1.0f,0.7f });
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
	}
}
