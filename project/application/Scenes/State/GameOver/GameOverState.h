#pragma once
//ゲームオーバーシーン//
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"

class GameOverState :public IGameState
{
public:
	GameOverState() { StateNo = GameStateNo::GAMEOVER; };
	void Init()override;
	void Update()override;
	void Draw()override;
private:
	TextureManager* textureManager_;

	std::unique_ptr<Sprite>texture;
	WorldTransform texture_world_;

	bool IsCanPush = false;
	int time = 0;
};
