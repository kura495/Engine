#include "Fade.h"

void Fade::InInit()
{
	Init();
	alpha_ = 0.0f;
	sprite_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });
}

void Fade::OutInit()
{
	Init();
	alpha_ = 1.0f;
	sprite_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });
}

bool Fade::In(float second)
{
	//アルファを徐々に上げていく
	timeCount_ += kDeltaTime;
	alpha_ = (1.0f / second) * timeCount_;

	sprite_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });

	if (timeCount_ >= second) {
		return true;
	}
	else {
		return false;
	}
}
bool Fade::Out(float second)
{
	//アルファを徐々に下げていく
	timeCount_ += kDeltaTime;
	alpha_ = 1.0f - (1.0f / second) * timeCount_;

	sprite_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });

	if (timeCount_ >= second) {
		return true;
	}
	else {
		return false;
	}
}
void Fade::Init()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	sprite_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/BlackTexture.png");
	world_.Init();
}
void Fade::Draw()
{
	sprite_->RendererDraw(world_);
}
