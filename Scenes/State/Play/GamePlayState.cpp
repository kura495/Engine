#include "Scenes/State/Play/GamePlayState.h"

void GamePlayState::Initialize()
{
	StateNo = GameStateNo::PLAY;

	//基本機能生成
	debugcamera_ = new DebugCamera();
	debugcamera_->Initialize();
	Editer::GetInstance()->SetViewProjection(&Renderer::viewProjection);
	Editer::GetInstance()->IsEnable(true);
	objectManager = ObjectManager::GetInstance();

	objectManager->LordBlenderScene("resources/Stage");

	collisionManager = std::make_unique<CollisionManager>();
	collisionManager->Init();
	//
	//3Dオブジェクト生成
	playerModel_.push_back(Model::CreateModelFromObj("resources/human", "walk.gltf"));
	playerModel_.push_back(Model::CreateModelFromObj("resources/Weapon", "Weapon.obj"));
	lanthanumModel_.push_back(Model::CreateModelFromObj("resources/Object", "Object.obj"));
	lanthan.Initialize();

	titleSprite = new Sprite;
	titleSprite->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	titleSprite->TextureHandle = TextureManager::GetInstance()->LoadTexture("resources/Title.png");
	title.Initialize();
	//title.transform.translate.z = -1.0f;
	//title.Update();

	player_ = std::make_unique<Player>();
	player_->Init(playerModel_);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();


	lockOn.Init();
	followCamera->SetLockOn(&lockOn);
	player_->SetLockOn(&lockOn);

	//Renderer
	renderer_ = Renderer::GetInstance();

	particle = new ParticleSystem();
	particle->Initalize("resources/circle2.png");

	//enemyManager
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Init(player_.get());

	input = Input::GetInstance();

	//天球
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

}

void GamePlayState::Update()
{
	BehaviorUpdate();

	particle->Update();

	skyDome_->Update();

	followCamera->Update();
	Renderer::viewProjection = followCamera->GetViewProjection();

	if (enemyManager->GetisClear()) {
		StateNo = 2;
	}
	if (player_->GetisDead()) {
		StateNo = 3;
	}
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

#pragma region Game

	objectManager->Draw();



	enemyManager->Draw();

#pragma endregion

	if (behavior_ == StageBehavior::kPlay) {

		player_->Draw();
	}
	else {
		TitleDraw();
	}


	for (Model* model : lanthanumModel_) {
		model->RendererDraw(lanthan);
	}

	skyDome_->Draw();

	collisionManager->Draw();

	//particle->RendererDraw();

}
#pragma region
void GamePlayState::BehaviorUpdate()
{
#pragma region
	//初期化
	if (behaviorRequest_) {
		//ふるまいの変更
		behavior_ = behaviorRequest_.value();
		//各ふるまいごとに初期化
		switch (behavior_)
		{
		case StageBehavior::kTitle:
		default:
			TitleInit();
			break;
		case StageBehavior::kPlay:
			PlayInit();
			break;
		case StageBehavior::kClear:
			ClearInit();
			break;
		case StageBehavior::kOver:
			OverInit();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}
	//更新
	switch (behavior_)
	{
	case StageBehavior::kTitle:
	default:
		TitleUpdate();
		break;
	case StageBehavior::kPlay:
		PlayUpdate();
		break;
	case StageBehavior::kClear:
		ClearUpdate();
		break;
	case StageBehavior::kOver:
		OverUpdate();
		break;
	}
#pragma endregion BehaviorTree
}

#pragma region
void GamePlayState::TitleInit()
{

}
void GamePlayState::TitleUpdate()
{
	if (input->IsTriggerPad(XINPUT_GAMEPAD_A) || input->IsTriggerKey(DIK_SPACE)) {
		IsTitleToGameFlag = true;
	}
	if (IsTitleToGameFlag) {
		if (followCamera->PlaySceneInit(&player_->GetWorld())) {
			behaviorRequest_ = StageBehavior::kPlay;

		}
	}
	player_->TitleUpdate();
}

void GamePlayState::TitleDraw()
{
	titleSprite->RendererDraw(title);
	player_->TitleDraw();
}

#pragma endregion Title
#pragma region
void GamePlayState::PlayInit()
{
	IsTitleToGameFlag = false;
}

void GamePlayState::PlayUpdate()
{
	player_->Update();
	enemyManager->Update();

	collisionManager->Update();

	lockOn.Update(enemyManager->GetList());
}

#pragma endregion Play
#pragma region
void GamePlayState::ClearInit()
{
}

void GamePlayState::ClearUpdate()
{
}

#pragma endregion Clear
#pragma region
void GamePlayState::OverInit()
{
}

void GamePlayState::OverUpdate()
{
}

#pragma endregion Over
#pragma endregion Behavior