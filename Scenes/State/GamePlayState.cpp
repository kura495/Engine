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

	DirectX_ = DirectXCommon::GetInstance();

	sphereTexture = textureManager_->LoadTexture("resources/monsterBall.png");

	world_.Initialize();

	sphere = std::make_unique<Sphere>();
	sphere->Initialize();
}

void GamePlayState::Update()
{
	world_.UpdateMatrix();
	debugcamera_->Update();
#ifdef _DEBUG

	ImGui::Begin("Config");
	if (ImGui::TreeNode("Camera")) {
		ImGui::DragFloat3("Rotate", &debugcamera_->GetViewProjection().rotation_.x,0.1f);
		ImGui::DragFloat3("Translate", &debugcamera_->GetViewProjection().translation_.x);
		if (ImGui::Button("Reset")) {
			debugcamera_->GetViewProjection().rotation_ = { 0.0f,0.0f,0.0f };
			debugcamera_->GetViewProjection().translation_ = { 0.0f,0.0f,-10.0f };
			debugcamera_->GetViewProjection().UpdateMatrix();
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Light")) {
		ImGui::SliderFloat4("LightColor", &light_->GetLightData().color.x, 0, 1, "%.3f");
		ImGui::SliderFloat3("Lightpotision", &light_->GetLightData().direction.x, -10, 10, "%.3f");
		ImGui::SliderFloat("Lightpotision", &light_->GetLightData().intensity, -10, 10, "%.3f");
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Object")) {
		ImGui::DragFloat3("translate",&world_.transform_.translate.x);
		ImGui::TreePop();
	}

	ImGui::End();
#endif
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

	sphere->Draw(world_, debugcamera_->GetViewProjection(), sphereTexture);

	//3Dモデル描画ここまで	
	

	//Sprite描画ここから

	//Sprite描画ここまで
	
	//パーティクル描画ここから

	//パーティクル描画ここまで

	//描画ここまで
}



