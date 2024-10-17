#pragma once
#include "Texture/TextureManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Sprite/Sprite.h"
class Fade
{
public:
	static Fade* GetInstance();

	void InInit();
	void OutInit();
	bool In();
	bool Out();
	void Draw();

private:
	Fade() {
		sprite_ = new Sprite;
		sprite_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
		sprite_->TextureHandle = TextureManager::GetInstance()->LoadTexture("resources/BlackTexture.png");
		world_.Initialize();
	}
	~Fade() = default;
	Fade(const Fade& obj) = delete;
	Fade& operator=(const Fade& obj) = delete;


	Sprite* sprite_;
	WorldTransform world_;

	float alpha_ = 0.0f;

};