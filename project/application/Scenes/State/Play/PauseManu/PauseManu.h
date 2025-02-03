#pragma once
#include "Input.h"
#include "Texture/TextureManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Sprite/Sprite.h"
class PauseManu {
public:
	void Init();
	bool Update();
	void Draw();
	bool GetIsPause() { return IsPause; };
private:
	bool IsPause = false;

	std::unique_ptr<Sprite> sprite_;
	WorldTransform world_;
};