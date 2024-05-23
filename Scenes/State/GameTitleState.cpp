#include"Scenes/State/GameTitleState.h"

void GameTitleState::Initialize()
{
	Editer::GetInstance()->IsEnable(false);
	input = Input::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	textureHundle = textureManager_->LoadTexture("resources/title.png");
	texture_world_.Initialize();

	textureHundle2 = textureManager_->LoadTexture("resources/BlackTexture.png");
	texture2_world_.Initialize();

	texture = std::make_unique<Sprite>();
	texture->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });

	texture2 = std::make_unique<Sprite>();
	texture2->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });

	time = 0;
	IsCanPush = false;
	FadeInFlag = false;
	FadeParam = 0.0f;
	t = 0.0f;
}

void GameTitleState::Update()
{
	time++;
	if (time > 60) {
		IsCanPush = true;
	}

	input->GetJoystickState(joyState);
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A ) {
		if (IsCanPush) {
			FadeInFlag = true;
		}
	}
	if (FadeInFlag) {
		FadeParam += 0.01f;
		if (FadeParam > 1.0f) {
			StateNo = 1;
		}
	}
	texture2->SetColor({ 1.0f,1.0f,1.0f,FadeParam });

	t += addParameter;
	if (t < 0.0f) {
		addParameter *= -1;
	}
	else if (t > 1.0f) {
		addParameter *= -1;
	}
	texture_world_.transform_.translate = Vector3::VectorLerp(Stert, End, t);

	texture_world_.UpdateMatrix();
}

void GameTitleState::Draw()
{
	texture->Draw(texture_world_, textureHundle);
	texture2->Draw(texture2_world_, textureHundle2);
}