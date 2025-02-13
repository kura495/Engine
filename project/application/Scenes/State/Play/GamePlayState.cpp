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

	ChangePhase<TitlePhase>();

}
void GamePlayState::Update()
{
	if (pauseMenu_->Update()) {
		return;
	}

	phase_->Update(this);

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
		ChangePhase<ClearPhase>();
	}
	if (player_->GetisDead()) {
		ChangePhase<GameOverPhase>();
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
	phase_->Draw(this);

#pragma endregion

	skyDome_->Draw();

	collisionManager->Draw();

	pauseMenu_->Draw();
}