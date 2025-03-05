#pragma once
#include "Sprite/Sprite.h"
class Life {
public:
	void Init(int HP);
	void Animation();
	void Draw();
private:
	TextureManager* textureManager_;

	std::unique_ptr<Sprite> sprite_Life;
	WorldTransform sprite_Life_World;
	std::unique_ptr<Sprite> sprite_Number;
	WorldTransform sprite_Number_World;
};