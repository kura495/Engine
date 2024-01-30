#include "Scenes/State/GamePlayState.h"

void GamePlayState::Initialize()
{
	//基本機能生成
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
	Editer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	input = Input::GetInstance();
	audio = Audio::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	light_ = Light::GetInstance();

	DirectX_ = DirectXCommon::GetInstance();
	collisionManager = std::make_unique<CollisionManager>();
	// 
	//3Dオブジェクト生成
	boxModel_.push_back( Model::CreateModelFromObj("resources/Cube/", "Cube.obj"));
	planeModel_.push_back( Model::CreateModelFromObj("resources/Plane/", "Plane.obj"));

	boxSelectNumber_ = 0;
	planeSelectNumber_ = 0;


	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	
	globalVariables = GlobalVariables::GetInstance();

	globalVariables->CreateGroup("Editer");
	
#pragma region
	globalVariables->AddItem("Editer", "BoxCount", boxObjectCount);
	boxObjectCount = globalVariables->GetIntValue("Editer", "BoxCount");
	for (int32_t boxit = 0; boxit < boxObjectCount;boxit++) {
		AddBox();
	}
	globalVariables->AddItem("Editer", "PlaneCount", PlaneObjectCount);
	PlaneObjectCount = globalVariables->GetIntValue("Editer", "PlaneCount");
	for (int32_t Pleneit = 0; Pleneit < PlaneObjectCount; Pleneit++) {
		AddPlane();
	}
#pragma endregion オブジェクト生成
}

void GamePlayState::Update()
{
	//デバッグカメラ
	debugCamera_->Update();
	viewProjction = debugCamera_->GetViewProjection();

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

#pragma region

	for (std::list<BoxObject*>::iterator ObjectIt = boxObject_.begin(); ObjectIt != boxObject_.end(); ObjectIt++) {
		(*ObjectIt)->Update();
	}
	for (std::list<PlaneObject*>::iterator ObjectIt = planeObject_.begin(); ObjectIt != planeObject_.end(); ObjectIt++) {
		(*ObjectIt)->Update();
	}
#pragma endregion Update

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



