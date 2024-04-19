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
	Editer::GetInstance()->SetViewProjection(&viewProjction);
	Editer::GetInstance()->IsEnable(true);
	objectManager = ObjectManager::GetInstance();
	//objectManager->LordFile("Editer");

	DirectX_ = DirectXCommon::GetInstance();

	world_.Initialize();

	Cube = new Model();
	Cube->Initialize("resources/AnimatedCube", "AnimatedCube.gltf");
	animation = new Animation();
	*animation = Animation::LoadAnimationFile("resources/AnimatedCube", "AnimatedCube.gltf");
}

void GamePlayState::Update()
{
		debugcamera_->Update();
		viewProjction = debugcamera_->GetViewProjection();

		animationTime += 1.0f / 60.0f;
		animationTime = std::fmod(animationTime, animation->duration);

		NodeAnimation& rootNodeAnimation = animation->nodeAnimations[Cube->GetModelData().rootNode.name];
		Vector3 translate = Animation::CalculateValue(rootNodeAnimation.translate.keyFrames,animationTime);
		Quaternion rotation = Animation::CalculateValue(rootNodeAnimation.rotate.keyFrames,animationTime);
		Vector3 scale = Animation::CalculateValue(rootNodeAnimation.scale.keyFrames,animationTime);
		world_.transform_.translate += translate;
		world_.transform_.quaternion += rotation;
		world_.transform_.scale = scale;
		world_.UpdateMatrix();
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから
	objectManager->Draw(viewProjction);
	Cube->Draw(world_,viewProjction);

	//3Dモデル描画ここまで	


	//Sprite描画ここから

	//Sprite描画ここまで

	//パーティクル描画ここから

	//パーティクル描画ここまで

	//描画ここまで
}
