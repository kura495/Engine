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
	renderer_ = std::make_unique<Renderer>();
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
	//グローバル変数読み込み
	GlobalVariables::GetInstance()->LoadFiles();
	//State
	state[TITLE] = std::make_unique<GameTitleState>();
	state[PLAY] = std::make_unique<GamePlayState>();
	state[CLEAR] = std::make_unique<GameClearState>();
	state[TITLE]->Initialize();
	currentSceneNum_ = TITLE;
}
void GameManager::Gameloop()
{
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			prevSceneNum_ = currentSceneNum_;
			currentSceneNum_ = state[currentSceneNum_]->GetSceneNum();

			if (prevSceneNum_ != currentSceneNum_)
			{
				state[currentSceneNum_]->Initialize();
			}
			imGuiManager->BeginFrame();
			directX->PreView();
			editer->Update();
			input->Update();
			light->Update();
			GlobalVariables::GetInstance()->Update();
			state[currentSceneNum_]->Update();
			renderer_->Draw(PipelineType::Standerd);
			state[currentSceneNum_]->Draw();
			editer->Draw();
			imGuiManager->EndFrame();
			directX->PostView();
		}
	}

}

void GameManager::Release()
{
	directX->Release();

	ImGui_ImplDX12_Shutdown();
	CoUninitialize();
}
