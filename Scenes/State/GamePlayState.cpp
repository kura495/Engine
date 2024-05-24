#include "Scenes/State/GamePlayState.h"

void GamePlayState::Initialize()
{
	// DEBUG : 消すように！！
	Vector3 VectorA(2, 2, 2);
	Vector3 VectorB(1, 2, 3);

	VectorA = VectorA - VectorB;
	
	/*Vector3 VectorC(2, 2, 2);
	float FloatB = 2.0f;

	VectorC *= FloatB;*/

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
	enemyModel_.push_back(Model::CreateModelFromObj("resources/human", "human.gltf"));
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

	//Renderer
	renderer_ = Renderer::GetInstance();

}

void GamePlayState::Update()
{
	debugcamera_->Update();
	viewProjction = debugcamera_->GetViewProjection();
	Renderer::viewProjection = debugcamera_->GetViewProjection();

#pragma region Game
	followCamera->Update();
	viewProjction = followCamera->GetViewProjection();
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
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

#pragma region Game

	objectManager->Draw();
	if (IsDebugCamera == true) {
		for (Model* model : playerModel_) {
			renderer_->AddModelData(*model,player_->GetWorldTransform());
		}
	}

	player_->Draw();


	for (Enemy* enemy : enemys_) {
		enemy->Draw();
	}

	for (Enemy* enemy : enemys_) {
		enemy->DabugDraw();
	}

	renderer_->Draw();

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
