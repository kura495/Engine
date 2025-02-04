#pragma once
//画面遷移クラス//
#include "Texture/TextureManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Sprite/Sprite.h"
class Fade
{
public:
	static Fade* GetInstance();
	//フェードインをする時の初期化
	void InInit();
	//フェードアウトをする時の初期化
	void OutInit();
	//フェードイン用更新処理　終了するとtrueを返す
	bool In();
	//フェードアウト用更新処理　終了するとtrueを返す
	bool Out();
	void Draw();

private:
	Fade() {
		sprite_ = new Sprite;
		sprite_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
		sprite_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/BlackTexture.png");
		world_.Init();
	}
	~Fade() = default;
	Fade(const Fade& obj) = delete;
	Fade& operator=(const Fade& obj) = delete;


	Sprite* sprite_;
	WorldTransform world_;

	float alpha_ = 0.0f;

};