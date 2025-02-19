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
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

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


	//play
	std::unique_ptr<CollisionManager> collisionManager;
	std::unique_ptr<EnemyManager> enemyManager;

	std::unique_ptr<Player>player_;

	std::unique_ptr<FollowCamera>followCamera;
private:

	std::unique_ptr<GamePlayStatePhase> phase_;
	std::vector<Model*> playerModel_;
	//基本機能ズ
	ObjectManager* objectManager = nullptr;

	std::vector<Model*> boxModel_;
	std::vector<Model*> planeModel_;
	std::vector<Model*> glTFplaneModel_;

	std::unique_ptr<SkyDome> skyDome_;

	std::unique_ptr<FloorManager> floorManager;


	std::unique_ptr<PauseMenu>pauseMenu_;

};