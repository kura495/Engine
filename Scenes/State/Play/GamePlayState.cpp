#include "Scenes/State/Play/GamePlayState.h"

void GamePlayState::Initialize()
{
	StateNo = GameStateNo::PLAY;

	//基本機能生成
	debugcamera_ = std::make_unique<DebugCamera>();
	debugcamera_->Initialize();
	Editer::GetInstance()->SetViewProjection(&Renderer::viewProjection);
	Editer::GetInstance()->IsEnable(true);
	objectManager = ObjectManager::GetInstance();

	objectManager->LordBlenderScene("resources/Stage");

	collisionManager = std::make_unique<CollisionManager>();
	collisionManager->Init();
	//
	//3Dオブジェクト生成
	playerModel_.push_back(Model::CreateModelFromObj("resources/Player", "player.gltf"));
	playerModel_.push_back(Model::CreateModelFromObj("resources/Weapon", "Weapon.obj"));

	titleSprite = std::make_unique<Sprite>();
	titleSprite->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	titleSprite->TextureHandle = TextureManager::GetInstance()->LoadTexture("resources/Title.png");
	title.Initialize();

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();

	player_ = std::make_unique<Player>();
	player_->Init(playerModel_);

	//Renderer
	renderer_ = Renderer::GetInstance();

	particle = std::make_unique<ParticleSystem>();
	particle->Initalize("resources/circle2.png");

	//enemyManager
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Init(player_.get());

	input = Input::GetInstance();

	//天球
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	fade = Fade::GetInstance();
	fade->OutInit();

	behaviorRequest_ = StageBehavior::kTitle;

}

void GamePlayState::Update()
{
	BehaviorUpdate();

	followCamera->Update();
	Renderer::viewProjection = followCamera->GetViewProjection();

	if (enemyManager->GetSlamFlag()) {
		followCamera->isShake = true;
	}
	else {
		followCamera->isShake = false;
	}

	//particle->Update();

	skyDome_->Update();

	if (enemyManager->GetisClear()) {
		behaviorRequest_ = StageBehavior::kClear;
	}
	if (player_->GetisDead()) {
		behaviorRequest_ = StageBehavior::kOver;
	}
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

#pragma region Game

	objectManager->Draw();

	enemyManager->Draw();

#pragma endregion

#pragma region
	switch (behavior_)
	{
	case StageBehavior::kTitle:
	default:
		TitleDraw();
		break;
	case StageBehavior::kPlay:
		PlayDraw();
		break;
	case StageBehavior::kClear:
		ClearDraw();
		break;
	case StageBehavior::kOver:
		OverDraw();
		break;
	}

#pragma endregion

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
	if (fade->Out() == false) {
		return;
	}
#ifdef _DEBUG
	//TODO:デバッグ用なので消すこと！
	IsTitleToGameFlag = true;
#endif
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
	fade->Draw();
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
}
void GamePlayState::PlayDraw()
{
	player_->Draw();
}
#pragma endregion Play
#pragma region
void GamePlayState::ClearInit()
{
}
void GamePlayState::ClearUpdate()
{
	player_->Update();
	if (fade->In()) {
		StateNo = 2;
	}
}
void GamePlayState::ClearDraw()
{
	player_->Draw();
	fade->Draw();
}
#pragma endregion Clear
#pragma region
void GamePlayState::OverInit()
{

}
void GamePlayState::OverUpdate()
{
	player_->Update();
	if (fade->In()) {
		StateNo = 3;
	}
}

void GamePlayState::OverDraw()
{
	player_->Draw();
	fade->Draw();
}
#pragma endregion Over
#pragma endregion Behavior