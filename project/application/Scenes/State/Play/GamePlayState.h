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

#include "GamePlayPhase/TitlePhase/TitlePhase.h"
#include "GamePlayPhase/PlayPhase/PlayPhase.h"
#include "GamePlayPhase/ClearPhase/ClearPhase.h"
#include "GamePlayPhase/GameOverPhase/GameOverPhase.h"

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
	/// <summary>
	/// フェーズを切り替える
	/// </summary>
	template <typename T>
	void ChangePhase() {
		phase_ = std::make_unique<T>();
		//フェーズのすべての処理が終わる前にフェーズを切り替えてしまうと、
		//フェーズが変わって後の処理が出来なくなってエラーが出る
		phase_->Init(this);
	}
	std::unique_ptr<GamePlayStatePhase> phase_;
	//title
	int BGMHundle;
	float audioValue = 0.0f;
	float kMaxaudioValue = 0.04f;
	//ボタンを押したときに加算する。一定値を超えたらプレイフェーズに移行する
	float stertCount = 0.0f;
	const float subStertCount = 0.005f;
	//タイトル用画像
	std::unique_ptr<Sprite> titleSprite;
	WorldTransform title;
	//画面遷移演出
	Fade fade;
	//木箱のモデル
	std::vector<Model*> woodenBox;
	WorldTransform woodenBoxWorld_;
	//チュートリアル的なボタン表示
	std::vector<Model*> tutorialModel;
	WorldTransform tutorialWorld_;
	//play
	std::unique_ptr<CollisionManager> collisionManager;
	std::unique_ptr<EnemyManager> enemyManager;
	//over

	//clear

	//common
	//タイトルへ遷移するときのフラグ
	bool IsTitleToGameFlag = false;
	std::unique_ptr<Player>player_;
	std::vector<Model*> playerModel_;
	std::unique_ptr<FollowCamera>followCamera;
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

	//基本機能ズ
	std::unique_ptr<DebugCamera> debugcamera_;
	ObjectManager* objectManager = nullptr;

	Renderer* renderer_ = nullptr;

	bool IsDebugCamera = false;



	std::vector<Model*> boxModel_;
	std::vector<Model*> planeModel_;
	std::vector<Model*> glTFplaneModel_;

	std::unique_ptr<SkyDome> skyDome_;

	std::unique_ptr<FloorManager> floorManager;


	std::unique_ptr<PauseMenu>pauseMenu_;

};