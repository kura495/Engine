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

	std::unique_ptr<Sprite>background_;
	WorldTransform bg_world_;

	std::unique_ptr<Sprite>ui_;
	WorldTransform ui_world_;
	float ui_move_Value_;
	float ui_move_add_Value_ = 0.1f;
	float ui_move_MAX_ = 0.0f;
	float ui_move_MIN_ = -5.0f;

	bool IsCanPush = false;
	int time = 0;
};
