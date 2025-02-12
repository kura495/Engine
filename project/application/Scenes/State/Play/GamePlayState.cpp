#include "Scenes/State/Play/GamePlayState.h"

void GamePlayState::Init()
{
	StateNo = GameStateNo::PLAY;

	//基本機能生成
	debugcamera_ = std::make_unique<DebugCamera>();
	debugcamera_->Initialize();
	Editer::GetInstance()->SetViewProjection(&Renderer::GetViewProjection());
	Editer::GetInstance()->IsEnable(true);
	objectManager = ObjectManager::GetInstance();

	objectManager->LordBlenderScene("project/resources/Stage");

	collisionManager = std::make_unique<CollisionManager>();
	collisionManager->Init();
	//
	//3Dオブジェクト生成
	playerModel_.push_back(Model::CreateModelFromObj("project/resources/Player", "player.gltf"));
	playerModel_.push_back(Model::CreateModelFromObj("project/resources/Weapon", "Weapon.obj"));

	titleSprite = std::make_unique<Sprite>();
	titleSprite->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	titleSprite->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/Title.png");
	title.Init();

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();


	player_ = std::make_unique<Player>();
	player_->Init(playerModel_);
	
	followCamera->SetTarget(&player_->GetWorld());
	followCamera->Update();
	//Renderer
	renderer_ = Renderer::GetInstance();
	//enemyManager
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Init(player_.get());
	//天球
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	floorManager = std::make_unique<FloorManager>();
	floorManager->Init();

	fade.OutInit();

	behaviorRequest_ = StageBehavior::kTitle;

	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3",true);
	Audio::Play(BGMHundle, audioValue);

	woodenBox.push_back(Model::CreateModelFromObj("project/resources/Box", "Box.gltf"));

	woodenBoxWorld_.Init();
	woodenBoxWorld_.transform.translate.z = -3.0f;
	woodenBoxWorld_.Update();

	tutorialModel.push_back(Model::CreateModelFromObj("project/resources/Tutorial", "Tutorial.obj"));
	tutorialWorld_.Init();
	tutorialWorld_.transform.translate.z = -3.0f;
	tutorialWorld_.transform.translate.x = -2.0f;
	tutorialWorld_.Update();
	stertCount = 0.0f;
	woodenBoxWorld_.transform.scale.x = stertCount + 1.0f;
	woodenBoxWorld_.transform.scale.y = stertCount + 1.0f;
	woodenBoxWorld_.transform.scale.z = stertCount + 1.0f;
	woodenBoxWorld_.Update();

	pauseMenu_ = std::make_unique<PauseMenu>();
	pauseMenu_->Init();

}
void GamePlayState::Update()
{
	if (pauseMenu_->Update()) {
		return;
	}

	BehaviorUpdate();

	Renderer::SetViewProj(followCamera->GetParameter());

	if (enemyManager->GetSlamFlag()) {
		followCamera->isShake = true;
	}
	else {
		followCamera->isShake = false;
	}
	floorManager->Update();

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
	enemyManager->Draw();

	floorManager->Draw();
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

	pauseMenu_->Draw();
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
	audioValue = (std::min)(audioValue + 0.001f, kMaxaudioValue);
	Audio::Play(BGMHundle, audioValue);
	if (fade.Out() == false) {
		return;
	}
	followCamera->Update();
#ifdef _DEBUG
	ImGui::Begin("Title");
	ImGui::InputFloat("stertCount", &stertCount);
	ImGui::End();
#endif

	if (Input::GetPadPrecede(XINPUT_GAMEPAD_X,20)) {
		stertCount += 0.3f;

	}
	woodenBoxWorld_.Update();
	stertCount = (std::max)(stertCount - subStertCount,0.0f);

	woodenBoxWorld_.transform.scale.x = stertCount + 1.0f;
	woodenBoxWorld_.transform.scale.y = stertCount + 1.0f;
	woodenBoxWorld_.transform.scale.z = stertCount + 1.0f;

	if (stertCount >= 0.7f) {
		IsTitleToGameFlag = true;
	}
	if (IsTitleToGameFlag) {
		behaviorRequest_ = StageBehavior::kPlay;
	}
}
void GamePlayState::TitleDraw()
{
	titleSprite->RendererDraw(title);

	woodenBox[0]->RendererDraw(woodenBoxWorld_);
	tutorialModel[0]->RendererDraw(tutorialWorld_);

	fade.Draw();
}
#pragma endregion Title
#pragma region
void GamePlayState::PlayInit()
{
	IsTitleToGameFlag = false;
	player_->ChangeState<PJump>();
}
void GamePlayState::PlayUpdate()
{
	player_->Update();
	enemyManager->Update();
	collisionManager->Update();
	followCamera->Update();
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
	audioValue = (std::max)(audioValue - 0.001f,0.0f);
	Audio::Play(BGMHundle, audioValue);

	if (fade.In()) {
		StateNo = 2;
	}
}
void GamePlayState::ClearDraw()
{
	player_->Draw();
	fade.Draw();
}
#pragma endregion Clear
#pragma region
void GamePlayState::OverInit()
{

}
void GamePlayState::OverUpdate()
{
	player_->Update();
	audioValue = (std::max)(audioValue - 0.001f, 0.0f);
	Audio::Play(BGMHundle, audioValue);
	if (fade.In()) {
		StateNo = 3;
	}
}
void GamePlayState::OverDraw()
{
	player_->Draw();
	fade.Draw();
}
#pragma endregion Over
#pragma endregion Behavior