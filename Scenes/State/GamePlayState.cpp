#include "Scenes/State/GamePlayState.h"

void GamePlayState::Initialize()
{
	//基本機能生成
	camera_ = new Camera();
	camera_->Initialize(1280, 720);
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	light_ = Light::GetInstance();

	DirectX_ = DirectXCommon::GetInstance();

	//
	//3Dオブジェクト生成
	SpriteNum = textureManager_->LoadTexture("resources/uvChecker.png");
	sprite = new Sprite();
	sprite->Initialize({0.0f,0.0f},{0.0f,1200.0f},{1200.0f,0.0f},{1200.0f,1200.0f});
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

}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

	//3Dモデル描画ここまで	


	//Sprite描画ここから
	sprite->Draw(world_, SpriteNum);
	//Sprite描画ここまで
	
	//描画ここまで
}

