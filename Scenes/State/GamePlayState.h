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
#include "Base/Collider/Manager/CollisionManager.h"
#include "Base/Editer/Editer.h"
#include "Base/Object/Manaer/ObjectManager.h"
#include "Base/Animation/Animation.h"
#include "Base/Renderer/Renderer.h"
#include "Base/Object/Line/Line.h"

#include "Game/Object/Player/Player.h"
#include "Game/Object/Camera/FollowCamera.h"
#include "Game/Object/Enemy/Manager/EnemyManager.h"

class GamePlayState :public GameState
{
public:

	void Initialize();
	void Update();
	void Draw();

private:
	//基本機能ズ
	DebugCamera* debugcamera_ = nullptr;
	ObjectManager* objectManager = nullptr;
	TextureManager* textureManager_ = nullptr;
	std::unique_ptr<CollisionManager> collisionManager;
	Renderer* renderer_ = nullptr;

	bool IsDebugCamera = false;

	WorldTransform world_;

	std::unique_ptr<Player>player_;
	std::vector<Model*> playerModel_;
	std::unique_ptr<FollowCamera>followCamera;

	std::vector<Model*> boxModel_;
	std::vector<Model*> planeModel_;
	std::vector<Model*> glTFplaneModel_;
	std::vector<Model*> WeaponModel_;

	std::unique_ptr<EnemyManager> enemyManager;

	ParticleSystem* particle;
};