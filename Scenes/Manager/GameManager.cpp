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
	state[PLAY] = std::make_unique<GamePlayState>();
	state[CLEAR] = std::make_unique<GameClearState>();
	state[PLAY]->Initialize();
	currentSceneNum_ = PLAY;

	renderTextrue = new PostProsess();
	renderTextrue->Init();
	renderTextrue->Create(1);
	renderTextrue2 = new PostProsess();
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
			renderer_->ChangePipeline(PipelineType::PostProsessPSO);
			renderTextrue->Draw();
			renderer_->PostProsessDraw();
			renderTextrue->PostCopy();
			
			directX->PreView();
			//renderTargetを変更
			renderTextrue2->PreCopy();
			// TODO : ここにPipelineとDrawを書き込んでいく
			renderer_->ChangePipeline(PipelineType::PostProsessPSO);
			renderTextrue2->Draw();
			renderTextrue2->PostCopy();

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
