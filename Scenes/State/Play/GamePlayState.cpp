#include "Scenes/State/Play/GamePlayState.h"

void GamePlayState::Initialize()
{
	//基本機能生成
	debugcamera_ = new DebugCamera();
	debugcamera_->Initialize();
	Editer::GetInstance()->SetViewProjection(&Renderer::viewProjection);
	Editer::GetInstance()->IsEnable(true);
	objectManager = ObjectManager::GetInstance();

	objectManager->LordBlenderScene("resources/Stage");

	collisionManager = std::make_unique<CollisionManager>();
	collisionManager->Init();
	//
	//3Dオブジェクト生成
	playerModel_.push_back(Model::CreateModelFromObj("resources/human", "walk.gltf"));
	playerModel_.push_back(Model::CreateModelFromObj("resources/Weapon", "Weapon.obj"));

	player_ = std::make_unique<Player>();
	player_->Init(playerModel_);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->SetTarget(&player_->GetWorld());

	lockOn.Init();
	followCamera->SetLockOn(&lockOn);
	player_->SetLockOn(&lockOn);

	//Renderer
	renderer_ = Renderer::GetInstance();

	particle = new ParticleSystem();
	particle->Initalize("resources/circle.png");

	//enemyManager
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Init(player_.get());

		input = Input::GetInstance();

}

void GamePlayState::Update()
{

	player_->Update();
	enemyManager->Update();
	particle->Update();
	collisionManager->Update();

	lockOn.Update(enemyManager->GetList());

	followCamera->Update();
	Renderer::viewProjection = followCamera->GetViewProjection();

	if (enemyManager->GetisClear()) {
		StateNo = 2;
	}
	if (player_->GetisDead()) {
		StateNo = 3;
	}
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

#pragma region Game

	objectManager->Draw();

	player_->Draw();

	enemyManager->Draw();

#pragma endregion

	collisionManager->Draw();
	//3Dモデル描画ここまで	

	//Sprite描画ここから

	//Sprite描画ここまで

	//パーティクル描画ここから

	//particle->PreDraw();
	//particle->Draw(Renderer::viewProjection);

	//パーティクル描画ここまで

	//描画ここまで
}
