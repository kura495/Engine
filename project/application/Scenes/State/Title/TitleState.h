#pragma once
//ゲームクリアシーン//
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"
#include "Object/Transition/Fade/Fade.h"
#include "Object/SkyDome/SkyDome.h"
#include "Object/Camera/FollowCamera.h"
#include "ParticleSystem/ParticleSystem.h"
#include "UFO/UFO.h"

class TitleState :public IGameState
{
public:	
	enum UFOState {
		Idle,//停止状態
		Normal,//通常速度
		Boost,//最高速度の状態
	};
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
};
