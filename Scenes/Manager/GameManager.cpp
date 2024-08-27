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
	state[TITLE] = std::make_unique<GameTitleState>();
	state[PLAY] = std::make_unique<GamePlayState>();
	state[CLEAR] = std::make_unique<GameClearState>();
	state[PLAY]->Initialize();
	currentSceneNum_ = PLAY;
	//TODO : ちゃんとユニポとかにする
	renderTextrue = new PPFilter();
	renderTextrue->Init();
	renderTextrue->Create(1);
	renderTextrue2 = new PPGrayScale();
	renderTextrue2->Init();
	renderTextrue2->Create(2);
	renderTextrue3 = new PPVinette();
	renderTextrue3->Init();
	renderTextrue3->Create(3);
	renderTextrue4 = new PPLuminanceBasedOutline();
	renderTextrue4->Init();
	renderTextrue4->Create(4);
	renderTextrue5 = new PPRGBtoHSV();
	renderTextrue5->Init();
	renderTextrue5->Create(5);
	renderTextrue6 = new PPNormal();
	renderTextrue6->Init();
	renderTextrue6->Create(6);
}
void GameManager::Gameloop(){
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			prevSceneNum_ = currentSceneNum_;
			currentSceneNum_ = state[currentSceneNum_]->GetSceneNum();

			if (prevSceneNum_ != currentSceneNum_) {
				state[currentSceneNum_]->Initialize();
			}
			imGuiManager->BeginFrame();
		#pragma region Update
			editer->Update();
			objectManager->Update();
			input->Update();
			light->Update();
			GlobalVariables::GetInstance()->Update();
			state[currentSceneNum_]->Update();
			renderTextrue->Update();
			renderTextrue2->Update();
			renderTextrue3->Update();
			renderTextrue4->Update();
			renderTextrue5->Update();
			renderTextrue6->Update();
#pragma endregion
		#pragma region Draw
			//renderTextureに色々書き込んでいく
			renderTextrue->PreDraw();

			state[currentSceneNum_]->Draw();
			renderer_->Draw();
#pragma endregion
			//renderTargetを変更
			renderTextrue2->PreDraw();
			//レンダーターゲットから画像に変更
			renderTextrue->PreCopy();
			// TODO : ここにPipelineとDrawを書き込んでいく
			renderer_->ChangePipeline(PipelineType::GaussianFilter);
			renderTextrue->Draw();
			renderer_->PostProsessDraw();
			//画像からレンダーターゲット
			renderTextrue->PostCopy();

			renderTextrue3->PreDraw();
			renderTextrue2->PreCopy();
			renderer_->ChangePipeline(PipelineType::GrayScale);
			renderTextrue2->Draw();
			//renderer_->PostProsessDraw();
			renderTextrue2->PostCopy();

			renderTextrue4->PreDraw();
			renderTextrue3->PreCopy();
			renderer_->ChangePipeline(PipelineType::Vinette);
			renderTextrue3->Draw();
			//renderer_->PostProsessDraw();
			renderTextrue3->PostCopy();
			
			renderTextrue5->PreDraw();
			renderTextrue4->PreCopy();
			renderer_->ChangePipeline(PipelineType::LuminanceBasedOutline);
			renderTextrue4->Draw();
			//renderer_->PostProsessDraw();
			renderTextrue4->PostCopy();

			renderTextrue6->PreDraw();
			renderTextrue5->PreCopy();
			renderer_->ChangePipeline(PipelineType::RGBtoHSV);
			renderTextrue5->Draw();
			//renderer_->PostProsessDraw();
			renderTextrue5->PostCopy();

			directX->PreView();
			//renderTargetを変更
			renderTextrue6->PreCopy();
			renderer_->ChangePipeline(PipelineType::PostProsessPSO);
			renderTextrue6->Draw();
			renderTextrue6->PostCopy();

			editer->Draw();
			imGuiManager->EndFrame();
			directX->PostView();
		}
	}
}

void GameManager::Release(){
	directX->Release();

	ImGui_ImplDX12_Shutdown();
	CoUninitialize();
}
