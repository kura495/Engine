#include "Fade.h"

Fade* Fade::GetInstance()
{
	static Fade instance;
	return &instance;
}

void Fade::InInit()
{
	sprite_->SetColor({ 1.0f,1.0f,1.0f,0.0f });
	alpha_ = 0.0f;
}

void Fade::OutInit()
{
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
