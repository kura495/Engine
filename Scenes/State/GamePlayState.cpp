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
	sampleModel_ = std::make_unique<Model>();
	sampleModel_->Initialize("resources/Cube","Cube.obj");
	world_Model.Initialize();

	particle = std::make_unique<ParticleSystem>();
	particle->Initalize(10, "resources/circle.png");

	SpriteNum = textureManager_->LoadTexture("resources/uvChecker.png");
	sprite = new Sprite();
	sprite->Initialize({0.0f,0.0f},{0.0f,120.0f},{120.0f,0.0f},{120.0f,120.0f});
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



	camera_->Update();
	viewProjction = camera_->GetViewProjection();
 	particle->Update(viewProjction);
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから
	//sampleModel_->Draw(world_Model, viewProjction);
	//3Dモデル描画ここまで	


	//Sprite描画ここから
	sprite->Draw(world_, SpriteNum);
	//Sprite描画ここまで
	
	//パーティクル描画ここから
	particle->PreDraw();
	particle->Draw(viewProjction);
	//パーティクル描画ここまで

	//描画ここまで
}

