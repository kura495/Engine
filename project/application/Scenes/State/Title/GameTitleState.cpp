#include"Scenes/State/Title/GameTitleState.h"

void GameTitleState::Init()
{
	StateNo = GameStateNo::TITLE;

	Editer::GetInstance()->SetViewProjection(&Renderer::GetViewProjection());
	Editer::GetInstance()->IsEnable(false);
	input = Input::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	texture_world_.Init();
	texture = std::make_unique<Sprite>();
	texture->TextureHandle = textureManager_->LoadTexture("project/resources/title.png");
	texture->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });

	time = 0;
	IsCanPush = false;
	FadeInFlag = false;
}

void GameTitleState::Update()
{
	time++;
	if (time > 60) {
		IsCanPush = true;
	}
	if (input->GetPadPrecede(XINPUT_GAMEPAD_A,10)) {
		if (IsCanPush) {
			StateNo = 1;
		}
	}
}

void GameTitleState::Draw()
{
	texture->RendererDraw(texture_world_);
}