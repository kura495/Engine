#pragma once
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"
#include "Math/Vector/VectorCalc.h"

class GameTitleState :public GameState
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	
};