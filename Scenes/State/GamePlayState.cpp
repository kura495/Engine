#include "Scenes/State/GamePlayState.h"

void GamePlayState::Initialize()
{
	//基本機能生成
	camera_ = new Camera();
	camera_->Initialize();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	light_ = Light::GetInstance();

	DirectX_ = DirectXCommon::GetInstance();

	// 
	//3Dオブジェクト生成
	boxModel_.push_back( Model::CreateModelFromObj("resources/Cube/", "Cube.obj"));
	planeModel_.push_back( Model::CreateModelFromObj("resources/Plane/", "Plane.obj"));
	playerModel_.push_back( Model::CreateModelFromObj("resources/Player/", "Player.obj"));
	selectNumber_ = 0;

	plane_ = std::make_unique<Plane>();
	plane_->Initalize("resources/uvChecker.png");

	player_ = std::make_unique<Player>();
	player_->Initalize(playerModel_);
	
	globalVariables = GlobalVariables::GetInstance();

	globalVariables->CreateGroup("Editer");
	
	globalVariables->AddItem("Editer", "BoxCount", boxObjectCount);
	boxObjectCount = globalVariables->GetIntValue("Editer", "BoxCount");
	for (int32_t boxit = 0; boxit < boxObjectCount;boxit++) {
		AddBox();
	}

}

void GamePlayState::Update()
{
//デバッグカメラ
#ifdef _DEBUG
if (input->TriggerKey(DIK_LALT)) {
		camera_->DebugCamera(true);
}
else {
	camera_->DebugCamera(false);
}
#endif // _DEBUG
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
		}
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();

	ImGui::InputInt("Select", &selectNumber_);
	if (ImGui::Button("Delete")) {
		DeleteObject();
	}
	ControllObject();
}
ImGui::End();

//ImGui::Begin("Editer");
//if (ImGuizmo::IsUsing())
//{
//	ImGui::Text("Using gizmo");
//}
//else
//{
//	ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
//	ImGui::SameLine();
//	ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
//	ImGui::SameLine();
//	ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
//	ImGui::SameLine();
//	ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
//}
//ImGui::End();

#endif
//ImGuizmo
#ifdef _DEBUG
static ImGuiWindowFlags gizmoWindowFlags = 0;
ImGuiIO& io = ImGui::GetIO();
ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
Matrix4x4 IdentityMat = CreateIdentity4x4();
ImGuizmo::DrawGrid(&camera_->GetViewProjection().matView.m[0][0], &camera_->GetViewProjection().matProjection.m[0][0], &IdentityMat.m[0][0], 100.f);
for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
	if ((uint32_t)selectNumber_ == (*ObjectIt)->GetNumber()) {
	ImGuizmo::Manipulate(&camera_->GetViewProjection().matView.m[0][0], &camera_->GetViewProjection().matProjection.m[0][0], ImGuizmo::TRANSLATE, ImGuizmo::WORLD,&(*ObjectIt)->GetWorld().matWorld_.m[0][0]);
	(*ObjectIt)->GetWorld().transform_.translate = (*ObjectIt)->GetWorld().GetTranslateFromMatWorld();

	//ImGuizmo::ViewManipulate(&camera_->GetViewProjection().matView.m[0][0], 10, ImVec2(io.DisplaySize.x - 128, 0), ImVec2(128, 128), 0x10101010);

	break;
	}
}

#endif

	plane_->Update();
	player_->Update();
	camera_->Update();
	viewProjction = camera_->GetViewProjection();


}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		(*ObjectIt)->Draw(camera_->GetViewProjection());

	}
	plane_->Draw(camera_->GetViewProjection());
	player_->Draw(camera_->GetViewProjection());

	//3Dモデル描画ここまで	
	

	//Sprite描画ここから

	//Sprite描画ここまで
	
	//パーティクル描画ここから

	//パーティクル描画ここまで

	//描画ここまで
}

void GamePlayState::AddBox()
{
	IObject* box = new BoxObject;
	box->Initalize(boxModel_);

	std::string Number = std::to_string(box->GetNumber());

	std::string Name = "Box" + Number;
	globalVariables->AddItem("Editer",Name, box->GetWorld().transform_);

	box->SetTransform(globalVariables->GetTransformQuaValue("Editer", Name));

	if (selectNumber_ >= object_.size()) {
		object_.push_back(box);
		return;
	}

	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		if ((uint32_t)selectNumber_ == (*ObjectIt)->GetNumber()) {
			object_.insert(ObjectIt,box);
			return;
		}
	}
	object_.push_back(box);

}

void GamePlayState::AddPlane()
{
	IObject* plane = new PlaneObject;
	plane->Initalize(planeModel_);
	if (selectNumber_ >= object_.size()) {
		object_.push_back(plane);
		return;
	}
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		if ((uint32_t)selectNumber_ == (*ObjectIt)->GetNumber()) {
			object_.insert(ObjectIt, plane);
			break;
		}
	}
	object_.push_back(plane);
}

void GamePlayState::DeleteObject()
{

	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		if ((uint32_t)selectNumber_ == (*ObjectIt)->GetNumber()) {
			ObjectIt = object_.erase(ObjectIt);
			break;
		}
	}
}

void GamePlayState::LoadObject()
{

}

void GamePlayState::ControllObject()
{
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		if ((uint32_t)selectNumber_ == (*ObjectIt)->GetNumber()) {
			(*ObjectIt)->ImGui();
			(*ObjectIt)->Update();
			break;
		}
	}
}



