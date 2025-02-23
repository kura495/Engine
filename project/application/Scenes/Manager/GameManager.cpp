#include"GameManager.h"
void GameManager::Run()
{
	Initialize();
	Gameloop();
	Release();
}
void GameManager::Initialize()
{
	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);
	//Engine
	myEngine = MyEngine::GetInstance();
	myEngine->Initialize();
	//Window
	winApp = WinApp::GetInstance();
	//DirectX
	directX = DirectXCommon::GetInstance();
	//Renderer
	renderer_ = Renderer::GetInstance();
	renderer_->Initalize();
	//Audio
	audio = Audio::GetInstance();
	audio->Initialize();
	//Input
	input = Input::GetInstance();
	input->Initialize(winApp);
	//ImGui
	imGuiManager = ImGuiManager::GetInstance();
	imGuiManager->Initialize(winApp, directX);
	//TextureManager
	textureManager = TextureManager::GetInstance();
	//Light
	light = Light::GetInstance();
	light->Initialize();
	//Editer
	editer = Editer::GetInstance();
	editer->Initalize();
	//objectManager
	objectManager = ObjectManager::GetInstance();
	objectManager->Initalize();
	//グローバル変数読み込み
	GlobalVariables::GetInstance()->LoadFiles();
	//State
	state_ = std::make_unique<GamePlayState>();
	currentSceneNum_ = PLAY;
	state_->Init();

	renderTextrue = std::make_unique<PostProsess>();
	renderTextrue->Init();
	renderTextrue->Create(1);
	renderTextrue2 = std::make_unique<PostProsess>();
	renderTextrue2->Init();
	renderTextrue2->Create(2);
}
void GameManager::Gameloop(){
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			prevSceneNum_ = currentSceneNum_;
			currentSceneNum_ = state_->GetSceneNum();
			if (prevSceneNum_ != currentSceneNum_) {
				if (currentSceneNum_ == GameStateNo::PLAY) {
					state_ = std::make_unique<GamePlayState>();
				}
				if (currentSceneNum_ == GameStateNo::GAMEOVER) {
					state_ = std::make_unique<GameOverState>();
				}
				if (currentSceneNum_ == GameStateNo::CLEAR) {
					state_ = std::make_unique<GameClearState>();
				}
				state_->Init();
			}
			imGuiManager->BeginFrame();
#pragma region Update
			editer->Update();
			objectManager->Update();
			input->Update();
			light->Update();
			GlobalVariables::GetInstance()->Update();
			state_->Update();
			renderTextrue->Update();
			renderer_->Update();
#pragma endregion
#pragma region Draw
			//renderTextureに書き込む設定に変更
			renderTextrue->PreDraw();
			state_->Draw();
			renderer_->Draw();
			renderTextrue->PostDraw();
			renderTextrue2->PreDraw();
			//パイプラインの変更
			renderer_->ChangePipeline(PostProsessType::PostProsessPSO);
			//レンダーテクスチャの内容を書き込み
			renderTextrue->Draw();
			renderTextrue2->PostDraw();
			//directXのSRVに書き込む設定に変更
			directX->PreDraw();
			//ここにPipelineとDrawを書き込んでいく
			renderer_->ChangePipeline(PostProsessType::PostProsessPSO);
			renderTextrue2->Draw();
			editer->Draw();
			imGuiManager->EndFrame();
			directX->PostDraw();
			//流れと使い方
			//描画先A->PreDraw();
			//描画したい物->Draw();
			//描画先A->PostDraw();
			//描画先B->PreDraw();
			//描画先A->Draw();
			//描画先B->PostDraw();
			//directX->PreDraw();
			//描画先B->Draw();
			//directX->PostDraw();

#pragma endregion
		}
	}
}

void GameManager::Release(){
	directX->Release();

	ImGui_ImplDX12_Shutdown();
	CoUninitialize();
}
