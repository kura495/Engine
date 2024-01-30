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

#include "Base/Camera/DebugCamera/DebugCamera.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Math_Structs.h"
#include "Base/ParticleSystem/ParticleSystem.h"
#include "Base/Utility/CollisionManager.h"
#include "Base/Editer/Editer.h"

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
	TextureManager* textureManager_ = nullptr;	
	std::unique_ptr<DebugCamera>debugCamera_ = nullptr;
	Input* input = nullptr;
	Audio* audio=nullptr;
	Light* light_ = nullptr;
	DirectXCommon* DirectX_ = nullptr;
	GlobalVariables* globalVariables = nullptr;
	std::unique_ptr<CollisionManager> collisionManager;

	ViewProjection viewProjction;
	bool IsDebugCamera = false;


	std::unique_ptr<FollowCamera>followCamera;




	std::vector<Model*> boxModel_;
	std::vector<Model*> planeModel_;
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

};