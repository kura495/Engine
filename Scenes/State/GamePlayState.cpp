#include "Scenes/State/GamePlayState.h"

void GamePlayState::Initialize()
{

	//基本機能生成
	debugcamera_ = new DebugCamera();
	debugcamera_->Initialize();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	light_ = Light::GetInstance();
	Editer::GetInstance()->SetViewProjection(&Renderer::viewProjection);
	Editer::GetInstance()->IsEnable(true);
	objectManager = ObjectManager::GetInstance();
	//objectManager->LordFile("Editer");
	objectManager->LordBlenderScene("tools/test");

	DirectX_ = DirectXCommon::GetInstance();
	collisionManager = std::make_unique<CollisionManager>();
	collisionManager->Init();
	//
	//3Dオブジェクト生成
	enemyModel_.push_back(Model::CreateModelFromObj("resources/human", "human.gltf"));
	playerModel_.push_back(Model::CreateModelFromObj("resources/human", "walk.gltf"));
	playerModel_.push_back(Model::CreateModelFromObj("resources/Weapon", "Weapon.obj"));
	WeaponModel_.push_back(Model::CreateModelFromObj("resources/Weapon", "Weapon.obj"));

	player_ = std::make_unique<Player>();
	player_->Init(playerModel_);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera->GetViewProjection());

	enemys_.push_back(new Enemy);
	for (Enemy* enemy : enemys_) {
		enemy->Initialize(enemyModel_);
		enemy->SetPlayer(player_.get());
	}

	//Renderer
	renderer_ = Renderer::GetInstance();

	particle = new ParticleSystem();
	particle->Initalize("resources/circle.png");

	collisionManager->AddCollider(player_.get());
	for (Enemy* enemy : enemys_) {
		collisionManager->AddCollider(enemy);
	}
	//TODO : WeaponにフィルターをenemyとPlayerでよみとってないからあたる
	collisionManager->AddCollider(player_->GetWeapon());
}

void GamePlayState::Update()
{
	debugcamera_->Update();
	Renderer::viewProjection = debugcamera_->GetViewProjection();

#pragma region Game
	followCamera->Update();
	Renderer::viewProjection = followCamera->GetViewProjection();
#ifdef USE_IMGUI
		ImGui::Begin("Camera");
		if (ImGui::RadioButton("GameCamera", IsDebugCamera == false)) {
			IsDebugCamera = false;

		}
		if (ImGui::RadioButton("DebugCamera", IsDebugCamera == true)) {
			IsDebugCamera = true;

		}
		if (IsDebugCamera == true) {
			debugcamera_->Update();
			Renderer::viewProjection = debugcamera_->GetViewProjection();
		}
		ImGui::End();
#endif // _DEBUG
		player_->Update();
#pragma endregion 

	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	//particle->Update();


	collisionManager->Update();
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

#pragma region Game

	objectManager->Draw();

	player_->Draw();

	for (Enemy* enemy : enemys_) {
		enemy->Draw();
	}

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
