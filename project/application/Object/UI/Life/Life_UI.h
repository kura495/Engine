#pragma once
#include "Sprite/Sprite.h"
#include "application/Object/Transition/Fade/Fade.h"
class Life_UI {
public:
	void Init(int HP);
	bool Animation();
	void Draw();
private:

	Fade fade[3];

	WorldTransform sprite_world_[3];
	//テクスチャ
	TextureManager* textureManager_;
	std::unique_ptr<Sprite>sprite;
	//WorldTransform sprite_world_;
};