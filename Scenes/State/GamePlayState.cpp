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
	boxModel_.push_back(Model::CreateModelFromObj("resources/Cube/", "Cube.obj"));
	goalModel_.push_back(Model::CreateModelFromObj("resources/Goal/", "Goal.obj"));
	planeModel_.push_back(Model::CreateModelFromObj("resources/Plane/", "Plane.obj"));

	enemyModel_.push_back(Model::CreateModelFromObj("resources/Enemy/", "Enemy.obj"));
	playerModel_.push_back(Model::CreateModelFromObj("resources/Player/", "Player.obj"));
	WeaponModel_.push_back(Model::CreateModelFromObj("resources/Weapon/", "Weapon.obj"));
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
	globalVariables->AddItem("Editer", "BoxCount", boxObjectCount);
	boxObjectCount = globalVariables->GetIntValue("Editer", "BoxCount");
	for (int32_t boxit = 0; boxit < boxObjectCount; boxit++) {
		AddBox();
	}
	globalVariables->AddItem("Editer", "PlaneCount", PlaneObjectCount);
	PlaneObjectCount = globalVariables->GetIntValue("Editer", "PlaneCount");
	for (int32_t Pleneit = 0; Pleneit < PlaneObjectCount; Pleneit++) {
		AddPlane();
	}
#pragma endregion オブジェクト生成

	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_);

	FadeInFlag = true;
	FadeParam = 1.0f;
	textureHundle = textureManager_->LoadTexture("resources/BlackTexture.png");
	texture_world_.Initialize();

	texture = std::make_unique<Sprite>();
	texture->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
}

void GamePlayState::Update()
{
	//デバッグカメラ
	followCamera->Update();
	viewProjction = followCamera->GetViewProjection();
	//デバッグカメラ
#ifdef _DEBUG
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
//ImGui
#ifdef _DEBUG
ImGui::Begin("CreateObject",nullptr,ImGuiWindowFlags_MenuBar);
if (ImGui::BeginMenuBar()) {
	if (ImGui::BeginMenu("Box")) {
			
		if (ImGui::Button("Add Box")) {
			AddBox();
			boxObjectCount++;
			globalVariables->Updateint32_tItem("Editer","BoxCount", boxObjectCount);
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Plane")) {

		if (ImGui::Button("Add Plane")) {
			AddPlane();
			PlaneObjectCount++;
			globalVariables->Updateint32_tItem("Editer", "PlaneCount", PlaneObjectCount);
		}
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
}
ImGui::End();
#endif

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

	if (IsDebugCamera == true) {
		for (Model* model : playerModel_) {
			model->Draw(player_->GetWorldTransform(), viewProjction);
		}
	}
	goal_->Draw(viewProjction);

	player_->Draw(viewProjction);

	texture->Draw(texture_world_, textureHundle);

	//3Dモデル描画ここまで	
	

	//Sprite描画ここから

	//Sprite描画ここまで
	
	//パーティクル描画ここから

	//パーティクル描画ここまで

	//描画ここまで
}

void GamePlayState::AddBox()
{
	BoxObject* box = new BoxObject;
	box->Initalize(boxModel_);

	std::string Number = std::to_string(box->GetNumber());

	std::string Name = "Box" + Number;
	globalVariables->AddItem("Editer",Name, box->GetWorld().transform_);

	box->SetTransform(globalVariables->GetTransformQuaValue("Editer", Name));

		boxObject_.push_back(box);
}
void GamePlayState::AddPlane()
{
	PlaneObject* plane = new PlaneObject;
	plane->Initalize(planeModel_);

	std::string Number = std::to_string(plane->GetNumber());

	std::string Name = "Plane" + Number;
	globalVariables->AddItem("Editer", Name, plane->GetWorld().transform_);

	plane->SetTransform(globalVariables->GetTransformQuaValue("Editer", Name));

		planeObject_.push_back(plane);
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



