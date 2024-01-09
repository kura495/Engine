#pragma once
#include "IgameState.h"
#include "MyEngine.h"
#include "ModelData.h"
#include "Audio.h"
#include "Input.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Light.h"
#include "Sphere.h"
#include "TextureManager.h"
#include "Model.h"
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Base/Camera/Camera.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Math_Structs.h"
#include "Base/ParticleSystem/ParticleSystem.h"
#include "Base/Utility/CollisionManager.h"

#include "Game/Object/Box/BoxObject.h"
#include "Game/Object/Plane/PlaneObject.h"
#include "Game/Object/Player/Player.h"
#include "Game/Object/Camera/FollowCamera.h"
#include "Game/Object/Enemy/Enemy.h"
#include "Game/Object/Goal/Goal.h"

class GamePlayState :public GameState
{
public:

	void Initialize();
	void Update();
	void Draw();

private:
	//基本機能ズ
	MyEngine* myEngine = nullptr;
	Camera* camera_ = nullptr;
	TextureManager* textureManager_ = nullptr;	
	Input* input = nullptr;
	Audio* audio=nullptr;
	Light* light_ = nullptr;
	DirectXCommon* DirectX_ = nullptr;
	GlobalVariables* globalVariables = nullptr;
	std::unique_ptr<CollisionManager> collisionManager;

	ViewProjection viewProjction;
	bool IsDebugCamera = false;

	WorldTransform world_;

	std::unique_ptr<Player>player_;
	std::vector<Model*> playerModel_;
	std::unique_ptr<FollowCamera>followCamera;

	void AddEnemy(Vector3 Pos);

	std::vector<Model*> enemyModel_;
	std::list<Enemy*> enemy_;

	std::vector<Model*> boxModel_;
	std::vector<Model*> planeModel_;
	std::vector<Model*> WeaponModel_;
	void ControllObject();
	void DeleteObject();

	void AddBox();
	int32_t boxObjectCount;
	int32_t PlaneObjectCount;

	void AddPlane();

	std::list<BoxObject*> boxObject_;
	std::list<PlaneObject*> planeObject_;
	int boxSelectNumber_;
	int planeSelectNumber_;

	std::unique_ptr<Goal>goal_;

};