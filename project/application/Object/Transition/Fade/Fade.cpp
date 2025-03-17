#include "Fade.h"

void Fade::InInit(std::string FilePath)
{
	Init(FilePath);
	alpha_ = 0.0f;
	sprite_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });
}

void Fade::OutInit(std::string FilePath)
{
	Init(FilePath);
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
void Fade::Init(std::string FilePath)
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	sprite_->TextureHandle = TextureManager::GetInstance()->LoadTexture(FilePath);
	world_.Init();
}
void Fade::Draw()
{
	sprite_->RendererDraw(world_);
}
void Fade::Draw(WorldTransform& world)
{
	sprite_->RendererDraw(world);
}
