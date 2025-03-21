#include "Scenes/State/Play/GamePlayState.h"

void GamePlayState::Init()
{
	StateNo = GameStateNo::PLAY;

	//基本機能生成
	Editer::GetInstance()->SetViewProjection(&Renderer::GetViewProjection());
	Editer::GetInstance()->IsEnable(true);
	objectManager = ObjectManager::GetInstance();

	objectManager->LordBlenderScene("project/resources/Stage");
	//
	//3Dオブジェクト生成
	playerModel_.push_back(Model::CreateModelFromObj("project/resources/Player", "player.gltf"));
	playerModel_.push_back(Model::CreateModelFromObj("project/resources/Weapon", "Weapon.obj"));

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	//コリジョンマネージャー
	collisionManager = std::make_unique<CollisionManager>();
	collisionManager->Init();

	player_ = std::make_unique<Player>();
	player_->Init(playerModel_);
	
	followCamera->SetTarget(&player_->GetWorld());
	followCamera->Update();
	//enemyManager
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Init(player_.get());
	//天球
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	floorManager = std::make_unique<FloorManager>();
	floorManager->Init();

	pauseMenu_ = std::make_unique<PauseMenu>();
	pauseMenu_->Init();

	ChangePhase<TitlePhase>();

}
void GamePlayState::Update()
{
	if (pauseMenu_->Update()) {
		return;
	}

	phase_->Update(this);

	Renderer::SetViewProj(followCamera->GetParameter());

	if (enemyManager->GetSlamFlag()) {
		followCamera->isShake = true;
	}
	else {
		followCamera->isShake = false;
	}
	floorManager->Update();

	skyDome_->Update();
}
void GamePlayState::Draw()
{
	//3Dモデル描画ここから

#pragma region Game
	enemyManager->Draw();

	floorManager->Draw();
#pragma endregion

#pragma region
	phase_->Draw(this);

#pragma endregion

	skyDome_->Draw();

	collisionManager->Draw();

	pauseMenu_->Draw();
}

void GamePlayState::ReStert()
{
	player_->ReStert();
	floorManager->ReStert();
	enemyManager->ReStert();
	followCamera->ReStert();

	collisionManager->Update();
}
