#pragma once
//ゲームクリアシーン//
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"
#include "Object/Transition/Fade/Fade.h"
#include "Object/Transition/Fade/Fade.h"
#include "Object/SkyDome/SkyDome.h"
#include "Object/Camera/FollowCamera.h"
#include "ParticleSystem/ParticleSystem.h"
#include "UFO/UFO.h"

class TitleState :public IGameState
{
public:	
	TitleState() { StateNo = GameStateNo::TITLE; };
	void Init()override;
	void Update()override;
	void Draw()override;
private:
	std::unique_ptr<FollowCamera>followCamera;

	std::unique_ptr<UFO> ufo_;
	//天球
	std::unique_ptr<SkyDome> skyDome_;
	//振動の倍率
	int vibScale = 100;
	//カウント(フレーム)
	float countSecond = 0;
	//カウントの最大値(秒数)
	const float kMaxSecond = 2.0f;
	//カウントの倍率
	int countScale = 2;
	//画面遷移演出
	Fade fade;
	//タイトル用画像
	std::unique_ptr<Sprite> titleSprite;
	WorldTransform title;
};
