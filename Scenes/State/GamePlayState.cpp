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
	texturePath = textureManager_->LoadTexture("circle.png");

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

	world_.UpdateMatrix();
	testSirite->SetColor(Color);

	camera_->Update();
	viewProjction = camera_->GetViewProjection();
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

	//3Dモデル描画ここまで	


	//Sprite描画ここから
	testSirite->Draw(world_, texturePath);
	//Sprite描画ここまで
	
	//パーティクル描画ここから

	//パーティクル描画ここまで

	//描画ここまで
}

