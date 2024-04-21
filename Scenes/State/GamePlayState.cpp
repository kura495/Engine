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
	goalModel_.push_back(Model::CreateModelFromObj("resources/AnimatedCube", "AnimatedCube.gltf"));
	enemyModel_.push_back(Model::CreateModelFromObj("resources/Enemy", "Enemy.obj"));
	playerModel_.push_back(Model::CreateModelFromObj("resources/Player", "Player.obj"));
	WeaponModel_.push_back(Model::CreateModelFromObj("resources/Weapon", "Weapon.obj"));

	player_ = std::make_unique<Player>();
	player_->Initialize(WeaponModel_);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera->GetViewProjection());

	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_);



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


	goal_->Update();

#pragma region Game
		collisionManager->AddBoxCollider(player_.get());
		collisionManager->AddBoxCollider(player_->GetWeapon());
		for (std::list<Enemy*>::iterator ObjectIt = enemy_.begin(); ObjectIt != enemy_.end(); ObjectIt++) {
			collisionManager->AddBoxCollider((*ObjectIt));
			collisionManager->AddBoxCollider((*ObjectIt)->GetSearchPoint());
		}
		collisionManager->AddBoxCollider(goal_.get());
		collisionManager->CheckAllCollisions();
		collisionManager->ClearCollider();
#pragma endregion
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
	goal_->Draw(viewProjction);

	player_->Draw(viewProjction);
#pragma endregion

	//3Dモデル描画ここまで	


	//Sprite描画ここから

	//Sprite描画ここまで

	//パーティクル描画ここから

	//パーティクル描画ここまで

	//描画ここまで
}
