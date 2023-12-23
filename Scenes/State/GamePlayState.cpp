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
	selectNumber_ = 0;
	
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
#endif
//ImGuizmo
#ifdef _DEBUG
static ImGuiWindowFlags gizmoWindowFlags = 0;
for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
	if ((uint32_t)selectNumber_ == (*ObjectIt)->GetNumber()) {
		ImGui::Begin("Gizmo",0,gizmoWindowFlags);
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,windowWidth,windowHeight);
		ImGuizmo::Manipulate(&viewProjction.CameraMatrix.m[0][0],&viewProjction.matProjection.m[0][0],ImGuizmo::OPERATION::TRANSLATE,ImGuizmo::LOCAL,&(*ObjectIt)->GetWorld().matWorld_.m[0][0]);
		ImGui::End();
	break;
	}
}

#endif

	camera_->Update();
	viewProjction = camera_->GetViewProjection();


}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		(*ObjectIt)->Draw(camera_->GetViewProjection());

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
	IObject* box = new BoxObject;
	box->Initalize(boxModel_);

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

void GamePlayState::ControllObject()
{
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		if ((uint32_t)selectNumber_ == (*ObjectIt)->GetNumber()) {
			(*ObjectIt)->ImGui();
			//(*ObjectIt)->Update();
			break;
		}
	}
}



