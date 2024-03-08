#pragma once
#include"IgameState.h"
#include "Input/Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"
#include "VectorCalc.h"

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
	Vector3 Stert = { 0.0f,10.0f,0.0f };
	Vector3 End = { 0.0f,-10.0f,0.0f };

	std::unique_ptr<Sprite>texture2;
	uint32_t textureHundle2;
	WorldTransform texture2_world_;
	bool FadeInFlag = false;
	float FadeParam = 0.0f;


	Input* input;
	XINPUT_STATE joyState;

	bool IsCanPush = false;
	int time = 0;


};