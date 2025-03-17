#pragma once
#include "Sprite/Sprite.h"
#include "application/Object/Transition/Fade/Fade.h"
class Life_UI {
public:
	void Init(int HP);
	bool Animation();
	void Draw();
private:

	Fade fade1;
	Fade fade2;
	Fade fade3;

	WorldTransform sprite_world_1;
	WorldTransform sprite_world_2;
	WorldTransform sprite_world_3;
	//テクスチャ
	TextureManager* textureManager_;
	std::unique_ptr<Sprite>sprite;
	//WorldTransform sprite_world_;
};