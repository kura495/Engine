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
	Editer::GetInstance()->SetViewProjection(&viewProjction);
	Editer::GetInstance()->IsEnable(true);
	objectManager = ObjectManager::GetInstance();
	objectManager->LordFile("Editer");

	DirectX_ = DirectXCommon::GetInstance();
	collisionManager = std::make_unique<CollisionManager>();
	// 
	//3Dオブジェクト生成
	enemyModel_.push_back(Model::CreateModelFromObj("resources/human", "walk.gltf"));
	playerModel_.push_back(Model::CreateModelFromObj("resources/Player", "Player.obj"));
	WeaponModel_.push_back(Model::CreateModelFromObj("resources/Weapon", "Weapon.obj"));

	player_ = std::make_unique<Player>();
	player_->Initialize(WeaponModel_);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera->GetViewProjection());

	enemys_.push_back(new Enemy);
	for (Enemy* enemy : enemys_) {
		enemy->Initialize(enemyModel_);
		enemy->SetPlayer(player_.get());
	}

	//particle = new ParticleSystem();
	//particle->Initalize("resources/circle.png");
		//Renderer
	renderer_ = Renderer::GetInstance();

	line = new Line();
	line2 = new Line();
	line->Init();
	line2->Init();
	world_Line.Initialize();
	world_Line2.Initialize();
}

void GamePlayState::Update()
{
	debugcamera_->Update();
	viewProjction = debugcamera_->GetViewProjection();

#pragma region Game
	followCamera->Update();
	viewProjction = followCamera->GetViewProjection();
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
			viewProjction = debugcamera_->GetViewProjection();
		}
		ImGui::End();
#endif // _DEBUG
		player_->Update();
		player_->ImGui();
#pragma endregion 

	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	//particle->Update(viewProjction);

	line->Update({0.0f,0.0f,0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f});
	line2->Update({10.0f,10.0f,1.0f,1.0f},{100.0f,100.0f,100.0f,1.0f});
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

#pragma region Game

	objectManager->Draw(viewProjction);
	if (IsDebugCamera == true) {
		for (Model* model : playerModel_) {
			model->Draw(player_->GetWorldTransform(), viewProjction);
		}
	}

	player_->Draw(viewProjction);

	renderer_->Draw(PipelineType::Skinning);

	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjction);
	}

	renderer_->Draw(PipelineType::DrawLine);

	line->Draw(world_Line, viewProjction);
	line2->Draw(world_Line2, viewProjction);

#pragma endregion

	//3Dモデル描画ここまで	


	//Sprite描画ここから

	//Sprite描画ここまで

	//パーティクル描画ここから
	//particle->PreDraw();
	//particle->Draw(viewProjction);
	//パーティクル描画ここまで

	//描画ここまで
}
