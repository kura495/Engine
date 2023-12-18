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
	model_ = Model::CreateModelFromObj("resources/Cube/","Cube.obj");

	texturePath = textureManager_->LoadTexture("resources/circle.png");

	testSirite = std::make_unique<Sprite>();
	testSirite->Initialize({0.0f,0.0f},{64.0f,64.0f});
	world_.Initialize();
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
ImGui::Begin("Tex");
ImGuizmo::DecomposeMatrixToComponents(&world_.matWorld_.m[0][0], &world_.translation_.x, &world_.quaternion.x, &world_.scale_.x);
ImGui::DragFloat2("Translate",&world_.translation_.x);
ImGui::End();
#endif


	world_.UpdateMatrix();
	testSirite->SetColor(Color);

	camera_->Update();
	viewProjction = camera_->GetViewProjection();
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから


	model_->Draw(world_,viewProjction);
	//3Dモデル描画ここまで	

#ifdef _DEBUG
	ImGui::Begin("a");
	ImGuizmo::DrawCubes(&viewProjction.matView.m[0][0], &viewProjction.matProjection.m[0][0], &world_.matWorld_.m[0][0],1);
	ImGuizmo::DrawGrid(&viewProjction.matView.m[0][0], &viewProjction.matProjection.m[0][0], &world_.matWorld_.m[0][0],10);
	ImGui::End();
#endif

	//Sprite描画ここから
	//testSirite->Draw(world_, texturePath);
	//Sprite描画ここまで
	
	//パーティクル描画ここから

	//パーティクル描画ここまで

	//描画ここまで
}

