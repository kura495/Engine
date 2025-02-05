#include "Fade.h"

void Fade::Init()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	sprite_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/BlackTexture.png");
	world_.Init();
}

void Fade::InInit()
{
	Init();
	sprite_->SetColor({ 1.0f,1.0f,1.0f,0.0f });
	alpha_ = 0.0f;
}

void Fade::OutInit()
{
	Init();
	sprite_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	alpha_ = 1.0f;
}

bool Fade::In()
{
	//アルファを徐々に上げていく
	alpha_ = (std::min)(alpha_ + 0.01f,1.0f);
	sprite_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });

	if (alpha_ >= 1.0f) {
		return true;
	}
	else {
		return false;
	}
}

bool Fade::Out()
{
	//アルファを徐々に下げていく
	alpha_ = (std::max)(alpha_ - 0.01f, 0.0f);
	sprite_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });

	if (alpha_ <= 0.0f) {
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
