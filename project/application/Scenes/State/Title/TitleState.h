#pragma once
//ゲームクリアシーン//
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"
#include "Object/Transition/Fade/Fade.h"
#include "Object/SkyDome/SkyDome.h"

class TitleState :public IGameState
{
public:	
	TitleState() { StateNo = GameStateNo::TITLE; };
	void Init()override;
	void Update()override;
	void Draw()override;
private:

	//UFOのモデル
	std::unique_ptr<Model> UFOmodel_;
	WorldTransform UFOWorld_;

	std::unique_ptr<SkyDome> skyDome_;
};
