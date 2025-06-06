#pragma once
//ゲームクリアシーン//
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"
#include "Object/Transition/Fade/Fade.h"

class GameClearState :public IGameState
{
public:
	GameClearState() { StateNo = GameStateNo::CLEAR; };
	void Init()override;
	void Update()override;
	void Draw()override;
private:
	TextureManager* textureManager_;

	std::unique_ptr<Sprite>texture;
	uint32_t textureHundle;
	WorldTransform texture_world_;
	XINPUT_STATE joyState;

	bool IsCanPush = false;
	int time = 0;

	Fade fade;
};
