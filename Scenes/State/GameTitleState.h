#pragma once
#include"IgameState.h"
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
	TextureManager* textureManager_;

	std::unique_ptr<Sprite>texture;
	uint32_t textureHundle;
	WorldTransform texture_world_;

	float t = 0.0f;
	float addParameter = 0.01f;
	Vector3 Stert = { 0.0f,5.0f,0.0f };
	Vector3 End = { 0.0f,-5.0f,0.0f };

	bool FadeInFlag = false;
	float FadeParam = 0.0f;


	Input* input;
	XINPUT_STATE joyState;

	bool IsCanPush = false;
	int time = 0;


};