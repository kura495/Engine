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

	DirectX_ = DirectXCommon::GetInstance();
	collisionManager = std::make_unique<CollisionManager>();
	// 
	//3Dオブジェクト生成
	int Tex = textureManager_->LoadTexture("resources/Cube/Cube.png");
	Tex = Tex;
	goalModel_.push_back(Model::CreateModelFromObj("resources/Goal", "Goal.obj"));
	glTFplaneModel_.push_back(Model::CreateModelFromObj("resources/glTFPlane", "Plane.gltf"));
	PlaneObject* plane = new PlaneObject;
	plane->Initalize(glTFplaneModel_);
	glTFplaneObject_.push_back(plane);
	enemyModel_.push_back(Model::CreateModelFromObj("resources/Enemy", "Enemy.obj"));
	playerModel_.push_back(Model::CreateModelFromObj("resources/Player", "Player.obj"));
	WeaponModel_.push_back(Model::CreateModelFromObj("resources/Weapon", "Weapon.obj"));
	boxSelectNumber_ = 0;
	planeSelectNumber_ = 0;

	player_ = std::make_unique<Player>();
	player_->Initialize(WeaponModel_);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera->GetViewProjection());

	globalVariables = GlobalVariables::GetInstance();

	globalVariables->CreateGroup("Editer");

#pragma region
	
#pragma endregion オブジェクト生成

	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_);

	FadeInFlag = true;
	FadeParam = 1.0f;
	textureHundle = textureManager_->LoadTexture("resources/BlackTexture.png");
	texture_world_.Initialize();

	texture = std::make_unique<Sprite>();
	texture->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });

	particle = std::make_unique<ParticleSystem>();
	particle->Initalize("resources/circle.png");

	SoundHundle = audio->LoadAudio("resources/fanfare.wav",true);
}

void GamePlayState::Update()
{
	//デバッグカメラ
	followCamera->Update();
	viewProjction = followCamera->GetViewProjection();
	//デバッグカメラ
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


	if (FadeInFlag) {
		if (FadeParam > 0.0f) {
			FadeParam -= 0.01f;

		}
		if (FadeParam < 0.0f) {
			FadeInFlag = false;
		}
		texture->SetColor({ 1.0f,1.0f,1.0f,FadeParam });
	}


	if (player_->GetIsGoal()) {
		StateNo = 2;
	}

#pragma region

	player_->Update();

	for (std::list<BoxObject*>::iterator ObjectIt = boxObject_.begin(); ObjectIt != boxObject_.end(); ObjectIt++) {
		(*ObjectIt)->Update();
	}
	for (std::list<PlaneObject*>::iterator ObjectIt = planeObject_.begin(); ObjectIt != planeObject_.end(); ObjectIt++) {
		(*ObjectIt)->Update();
	}
	for (std::list<PlaneObject*>::iterator ObjectIt = glTFplaneObject_.begin(); ObjectIt != glTFplaneObject_.end(); ObjectIt++) {
		(*ObjectIt)->Update();
	}

	goal_->Update();
#pragma endregion Update
#pragma region
	collisionManager->AddBoxCollider(player_.get());
	collisionManager->AddBoxCollider(player_->GetWeapon());
	for (std::list<Enemy*>::iterator ObjectIt = enemy_.begin(); ObjectIt != enemy_.end(); ObjectIt++) {
		collisionManager->AddBoxCollider((*ObjectIt));
		collisionManager->AddBoxCollider((*ObjectIt)->GetSearchPoint());
	}
	for (std::list<PlaneObject*>::iterator ObjectIt = planeObject_.begin(); ObjectIt != planeObject_.end(); ObjectIt++) {
		collisionManager->AddBoxCollider((*ObjectIt));
	}
	for (std::list<BoxObject*>::iterator ObjectIt = boxObject_.begin(); ObjectIt != boxObject_.end(); ObjectIt++) {
		collisionManager->AddBoxCollider((*ObjectIt));
	}
	collisionManager->AddBoxCollider(goal_.get());
	collisionManager->CheckAllCollisions();
	collisionManager->ClearCollider();
#pragma endregion コリジョンマネージャーに登録
	particle->Update(viewProjction);
#ifdef USE_IMGUI
	ImGui::Begin("Sound");
	ImGui::SliderFloat("Volume",&audioVolume,0,1);
	ImGui::SliderInt("Pan",&pan,1,-1);
	if (ImGui::Button("Play")) {
		IsPlay = true;
	}
	if (ImGui::Button("Stop")) {
		IsPlay = false;
		audio->Stop(SoundHundle, false, LoopFlag);
	}
	ImGui::End();
#endif
	if (IsPlay == true) {
		audio->Play(SoundHundle, audioVolume,pan);
	}

}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

	for (std::list<BoxObject*>::iterator ObjectIt = boxObject_.begin(); ObjectIt != boxObject_.end(); ObjectIt++) {
		(*ObjectIt)->Draw(viewProjction);
	}
	for (std::list<PlaneObject*>::iterator ObjectIt = planeObject_.begin(); ObjectIt != planeObject_.end(); ObjectIt++) {
		(*ObjectIt)->Draw(viewProjction);
	}
	for (std::list<PlaneObject*>::iterator ObjectIt = glTFplaneObject_.begin(); ObjectIt != glTFplaneObject_.end(); ObjectIt++) {
		(*ObjectIt)->Draw(viewProjction);
	}

	if (IsDebugCamera == true) {
		for (Model* model : playerModel_) {
			model->Draw(player_->GetWorldTransform(), viewProjction);
		}
	}
	goal_->Draw(viewProjction);

	player_->Draw(viewProjction);

	//texture->Draw(texture_world_, textureHundle);

	particle->PreDraw();
	particle->Draw(viewProjction);



	//3Dモデル描画ここまで	


	//Sprite描画ここから

	//Sprite描画ここまで

	//パーティクル描画ここから

	//パーティクル描画ここまで

	//描画ここまで
}

void GamePlayState::DeleteObject()
{

	//for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
	//	if ((uint32_t)selectNumber_ == (*ObjectIt)->GetNumber()) {
	//		ObjectIt = object_.erase(ObjectIt);
	//		break;
	//	}
	//}
}