#include "Scenes/State/GamePlayState.h"

void GamePlayState::Initialize()
{
	//基本機能生成
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
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
ImGui::Begin("Object",nullptr,ImGuiWindowFlags_MenuBar);
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

	ImGui::InputInt("BoxSelect", &boxSelectNumber_);
	ImGui::InputInt("PlaneSelect", &planeSelectNumber_);
	if (ImGui::Button("Delete")) {
		DeleteObject();
	}
	ControllObject();
}
ImGui::End();
#endif
//ImGuizmo
#ifdef _DEBUG
Matrix4x4 GizmoMoveMatrix = CreateIdentity4x4();
ImGuiIO& io = ImGui::GetIO();
ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
Matrix4x4 IdentityMat = CreateIdentity4x4();
ImGuizmo::DrawGrid(&viewProjction.matView.m[0][0], &viewProjction.matProjection.m[0][0], &IdentityMat.m[0][0], 100.f);
//for (std::list<BoxObject*>::iterator ObjectIt = boxObject_.begin(); ObjectIt != boxObject_.end(); ObjectIt++) {
//	
	//if ((uint32_t)boxSelectNumber_ == (*ObjectIt)->GetNumber()) {
	//GizmoMoveMatrix = (*ObjectIt)->GetWorld().matWorld_;
	//ImGuizmo::Manipulate(&viewProjction.matView.m[0][0], &viewProjction.matProjection.m[0][0], ImGuizmo::TRANSLATE, ImGuizmo::WORLD,&GizmoMoveMatrix.m[0][0]);

//float scale[3], rotate[3], translate[3];
	//ImGuizmo::DecomposeMatrixToComponents(&GizmoMoveMatrix.m[0][0],translate,rotate,scale);
	//(*ObjectIt)->GetWorld().transform_.scale.x = scale[0];
	//(*ObjectIt)->GetWorld().transform_.scale.y = scale[1];
	//(*ObjectIt)->GetWorld().transform_.scale.z = scale[2];
	//(*ObjectIt)->GetWorld().transform_.translate.x = translate[0];
	//(*ObjectIt)->GetWorld().transform_.translate.y = translate[1];
	//(*ObjectIt)->GetWorld().transform_.translate.z = translate[2];
	//break;
	//}
//}
//for (std::list<PlaneObject*>::iterator ObjectIt = planeObject_.begin(); ObjectIt != planeObject_.end(); ObjectIt++) {
//	if ((uint32_t)planeSelectNumber_ == (*ObjectIt)->GetNumber()) {
//	ImGuizmo::Manipulate(&viewProjction.matView.m[0][0], &viewProjction.matProjection.m[0][0], ImGuizmo::TRANSLATE, ImGuizmo::WORLD,&(*ObjectIt)->GetWorld().matWorld_.m[0][0]);
//	break;
//	}
//}

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
void GamePlayState::ControllObject()
{
	for (std::list<BoxObject*>::iterator ObjectIt = boxObject_.begin(); ObjectIt != boxObject_.end(); ObjectIt++) {
		if ((uint32_t)boxSelectNumber_ == (*ObjectIt)->GetNumber()) {
			(*ObjectIt)->ImGui();
			break;
		}
	}
	for (std::list<PlaneObject*>::iterator ObjectIt = planeObject_.begin(); ObjectIt != planeObject_.end(); ObjectIt++) {
		if ((uint32_t)planeSelectNumber_ == (*ObjectIt)->GetNumber()) {
			(*ObjectIt)->ImGui();
			break;
		}
	}
}



