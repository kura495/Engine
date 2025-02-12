#pragma once
//ゲームプレイシーン//
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

#include "Camera/DebugCamera/DebugCamera.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Math_Structs.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Collider/Manager/CollisionManager.h"
#include "Editer/Editer.h"
#include "Object/Manager/ObjectManager.h"
#include "Animation/Animation.h"
#include "Renderer/Renderer.h"
#include "Object/Line/Line.h"

#include "Object/Player/Player.h"
#include "Object/Camera/FollowCamera.h"
#include "Object/Enemy/Manager/EnemyManager.h"
#include "Object/SkyDome/SkyDome.h"
#include "Object/Transition/Fade/Fade.h"
#include "Object/FloorManager/FloorManager.h"
#include "PauseMenu/PauseMenu.h"

enum class StageBehavior {
	kTitle,
	kPlay,
	kClear,
	kOver,
};

class GamePlayState : public IGameState
{
public:

	void Init()override;
	void Update()override;
	void BehaviorUpdate();
	void Draw()override;

private:
#pragma region 
	//ふるまい
	StageBehavior behavior_ = StageBehavior::kTitle;
	//次のふるまいリクエスト
	std::optional<StageBehavior> behaviorRequest_ = std::nullopt;
#pragma region
	void TitleInit();
	void TitleUpdate();
	void TitleDraw();
	bool IsTitleToGameFlag = false;
#pragma endregion Title
#pragma region
	void PlayInit();
	void PlayUpdate();
	void PlayDraw();
#pragma endregion Play
#pragma region
	void ClearInit();
	void ClearUpdate();
	void ClearDraw();
#pragma endregion Clear
#pragma region
	void OverInit();
	void OverUpdate();
	void OverDraw();
#pragma endregion Over

#pragma endregion Behavior
#pragma region
	int BGMHundle;
	float audioValue = 0.0f;
	float kMaxaudioValue = 0.04f;
#pragma endregion BGM
	//基本機能ズ
	std::unique_ptr<DebugCamera> debugcamera_;
	ObjectManager* objectManager = nullptr;
	std::unique_ptr<CollisionManager> collisionManager;
	Renderer* renderer_ = nullptr;

	bool IsDebugCamera = false;

	std::unique_ptr<Player>player_;
	std::vector<Model*> playerModel_;
	std::unique_ptr<FollowCamera>followCamera;

	std::vector<Model*> boxModel_;
	std::vector<Model*> planeModel_;
	std::vector<Model*> glTFplaneModel_;

	std::unique_ptr<EnemyManager> enemyManager;
	std::unique_ptr<SkyDome> skyDome_;

	std::unique_ptr<FloorManager> floorManager;

	std::vector<Model*> woodenBox;
	WorldTransform woodenBoxWorld_;
	
	std::vector<Model*> tutorialModel;
	WorldTransform tutorialWorld_;

	std::unique_ptr<PauseMenu>pauseMenu_;

	//ボタンを押したときに加算する。一定値を超えたらプレイステートに移行する
	float stertCount = 0.0f;
	const float subStertCount = 0.005f;

	std::unique_ptr<Sprite> titleSprite;
	WorldTransform title;

	Fade fade;

};