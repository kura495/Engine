#pragma once
//ゲームオーバーシーン//
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"

class GameOverState :public GameState
{
public:
	void Initialize();
	void Update();
	void Draw();
private:
	TextureManager* textureManager_;

	std::unique_ptr<Sprite>texture;
	uint32_t textureHundle;
	WorldTransform texture_world_;

	Input* input;
	XINPUT_STATE joyState;

	bool IsCanPush = false;
	int time = 0;
};
