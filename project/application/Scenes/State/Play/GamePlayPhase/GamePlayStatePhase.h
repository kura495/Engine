#pragma once
#include "Object/Model/Model.h"
#include "WorldTransform/WorldTransform.h"
#include "Object/Transition/Fade/Fade.h"
#include "Input.h"
#include "Texture/TextureManager.h"
#include "Object/Camera/FollowCamera.h"
class GamePlayState;
class GamePlayStatePhase {
public:
	GamePlayStatePhase() {};
	virtual ~GamePlayStatePhase() {};
	virtual void Init(GamePlayState* PlayState) = 0;
	virtual void Update(GamePlayState* PlayState) = 0;
	virtual void Draw(GamePlayState* PlayState) = 0;
private:
};