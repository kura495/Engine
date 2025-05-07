#include"Scenes/State/Title/TitleState.h"

void TitleState::Init()
{
	StateNo = GameStateNo::TITLE;

	Editer::GetInstance()->SetViewProjection(&Renderer::GetViewProjection());
	Editer::GetInstance()->IsEnable(true);

	//天球
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	ufo_ = std::make_unique<UFO>();
	ufo_->Init();

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->GetParameter().translation_ = {0.0f,0.0f,-10.0f};

	//フェードイン
	fade.InInit("project/resources/BlackTexture.png");

	titleSprite = std::make_unique<Sprite>();
	titleSprite->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	titleSprite->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/Title.png");
	title.Init();

	texture = std::make_unique<Sprite>();
	texture->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/TitleTuto.png");
	texture->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
}

void TitleState::Update()
{
	Renderer::SetViewProj(followCamera->GetParameter());

	//UFOの状態によってコントローラーをバイブレーションをさせる
	if (ufo_->GetState() == UFOState::Normal) {
		int vibValue = (int)Input::GetPadTrreger().x + (int)Input::GetPadTrreger().y;
		Input::VibrateController(vibValue * vibScale, vibValue * vibScale);
	}
	else if (ufo_->GetState() == UFOState::Boost) {
		int vibValue = (int)Input::GetPadTrreger().x + (int)Input::GetPadTrreger().y;
		//大きい振動を感じれるようにleftMotorを大きくする
		Input::VibrateController(vibValue * vibScale * vibScale, vibValue * vibScale);

		countSecond += kDeltaTime;
	}
	else if (ufo_->GetState() == UFOState::Idle) {
		//countFrameを0以下にしないために実装
		countSecond = (std::max)(0.0f, countSecond - kDeltaTime);
	}
	else if (ufo_->GetState() == UFOState::StartAnime) {
		if (fade.In(3.0f)) {
			StateNo = GameStateNo::PLAY;
		}
	}
	if (countSecond >= kMaxSecond) {
		if (ufo_->GetState() != UFOState::StartAnime) {
			ufo_->ChangeState<StartAnime>();
		}
	}

	ufo_->ImGui();

	skyDome_->Update();
	ufo_->Update();
}

void TitleState::Draw()
{
	skyDome_->Draw();
	ufo_->Draw();
	titleSprite->RendererDraw(title);
	texture->RendererDraw(title);

	fade.Draw();
}
