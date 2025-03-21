#pragma once
#include "Sprite/Sprite.h"
#include "application/Object/Transition/Fade/Fade.h"

class Life_UI {
public:
	void Init(int HP);
	bool Animation();
	void Draw();
private:
	enum Life_Count {
	ONE,
	TWO,
	TRHEE,
	END
	};
	int saveHP;
	Fade fade[Life_Count::END];
	WorldTransform sprite_world_[Life_Count::END];
	Math::Vector3 position[Life_Count::END] = {
		{-100.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{100.0f,0.0f,0.0f}, };
	//テクスチャ
	TextureManager* textureManager_;
	std::unique_ptr<Sprite>sprite;
	//WorldTransform sprite_world_;
};