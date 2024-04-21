#pragma once
#include "IgameState.h"
#include "Common/Engine/MyEngine.h"
#include "ModelData.h"
#include "Audio.h"
#include "Input.h"
#include "Object/Mesh/Mesh.h"
#include "Sprite/Sprite.h"
#include "Light/Light.h"
#include "Object/Sphere/Sphere.h"
#include "Texture/TextureManager.h"
#include "Object/Model/Model.h"
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Base/Camera/DebugCamera/DebugCamera.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Math_Structs.h"
#include "Base/ParticleSystem/ParticleSystem.h"
#include "Base/Utility/CollisionManager.h"
#include "Base/Editer/Editer.h"
#include "Base/Object/Manaer/ObjectManager.h"
#include "Base/Animation/Animation.h"

class GamePlayState :public GameState
{
public:

	void Initialize();
	void Update();
	void Draw();

private:
	//基本機能ズ
	DebugCamera* debugcamera_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	Light* light_ = nullptr;
	DirectXCommon* DirectX_ = nullptr;
	GlobalVariables* globalVariables = nullptr;
	ObjectManager* objectManager = nullptr;
	std::unique_ptr<CollisionManager> collisionManager;

	ViewProjection viewProjction;
	bool IsDebugCamera = false;

	float animationTime = 0.0f;
	WorldTransform world_;
	Model* Cube;
	Animation* animation;

	uint32_t audioHundle = 0;
	uint32_t audioHundle2;
};