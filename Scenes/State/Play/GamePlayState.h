#pragma once
#include "Scenes/State/IgameState.h"
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
#include "Game/Object/Camera/LockOn.h"
#include "Game/Object/SkyDome/SkyDome.h"

enum class StageBehavior {
	kTitle,
	kPlay,
	kClear,
	kOver,
};

class GamePlayState :public GameState
{
public:

	void Initialize();
	void Update();
	void BehaviorUpdate();
	void Draw();

private:
#pragma region 
	//ふるまい
	StageBehavior behavior_ = StageBehavior::kTitle;
	//次のふるまいリクエスト
	std::optional<StageBehavior> behaviorRequest_ = std::nullopt;
#pragma region
	void TitleInit();
	void TitleUpdate();
	bool IsTitleFlag = false;
#pragma endregion Title
#pragma region
	void PlayInit();
	void PlayUpdate();
#pragma endregion Play
#pragma region
	void ClearInit();
	void ClearUpdate();
#pragma endregion Clear
#pragma region
	void OverInit();
	void OverUpdate();
#pragma endregion Over

#pragma endregion Behavior

	//基本機能ズ
	DebugCamera* debugcamera_ = nullptr;
	ObjectManager* objectManager = nullptr;
	std::unique_ptr<CollisionManager> collisionManager;
	Renderer* renderer_ = nullptr;

	bool IsDebugCamera = false;

	std::unique_ptr<Player>player_;
	std::vector<Model*> playerModel_;
	std::unique_ptr<FollowCamera>followCamera;
	LockOn lockOn;

	std::vector<Model*> boxModel_;
	std::vector<Model*> planeModel_;
	std::vector<Model*> glTFplaneModel_;

	std::unique_ptr<EnemyManager> enemyManager;
	std::unique_ptr<SkyDome> skyDome_;

	ParticleSystem* particle;

	Input* input;

};