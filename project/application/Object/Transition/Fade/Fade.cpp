#include "Fade.h"

void Fade::Init()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	sprite_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/BlackTexture.png");
	world_.Init();
}

bool Fade::In(float addFlame, float endFlame)
{
	//アルファを徐々に上げていく
	timeCount_ += addFlame;
	alpha_ = timeCount_;

	sprite_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });

	if (timeCount_ >= endFlame) {
		return true;
	}
	else {
		return false;
	}
}

bool Fade::Out(float addFlame, float endFlame)
{
	//アルファを徐々に下げていく
	timeCount_ += addFlame;
	alpha_ = 1.0f - timeCount_;

	sprite_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });

	if (timeCount_ >= endFlame) {
		return true;
	}
	else {
		return false;
	}
}

void Fade::Draw()
{
	sprite_->RendererDraw(world_);
}
