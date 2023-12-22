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


#ifdef _DEBUG
	ImGui::Begin("Box");
	if (ImGui::Button("Add Box")) {
		AddBox();
	}
	if (ImGui::Button("Delete Box")) {
		DeleteBox();
	}
	//Boxの数字を選択
	ImGui::InputInt("Select",&selectNumber_);
	//選択したBoxの操作と更新
	ControllBox();
	ImGui::End();

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
	object_.push_back(box);
}

void GamePlayState::DeleteBox()
{

	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		if ((uint32_t)selectNumber_ == (*ObjectIt)->GetNumber()) {
			ObjectIt = object_.erase(ObjectIt);
			break;
		}
	}
}

void GamePlayState::ControllBox()
{
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		if ((uint32_t)selectNumber_ == (*ObjectIt)->GetNumber()) {
			(*ObjectIt)->ImGui();
			(*ObjectIt)->Update();
			break;
		}
	}
}

void GamePlayState::AddPlane()
{

}

void GamePlayState::DeletePlane()
{

}

void GamePlayState::ControllPlane()
{

}

