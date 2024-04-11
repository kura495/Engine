#pragma once
#include"Common/WinApp/WinApp.h"
#include"Common/DirectX/DirectXCommon.h"
#include"Common/Engine/MyEngine.h"
#include"Object/Mesh/Mesh.h"
#include"Texture/TextureManager.h"
#include"Light/Light.h"
#include"ImGuiManager.h"
#include"Scenes/State/GameTitleState.h"
#include"Scenes/State/GamePlayState.h"
#include"Scenes/State/GameClearState.h"
#include"Scenes/State/IgameState.h"
#include"Audio.h"
#include"Input.h"
#include"GlobalVariables.h"
#include "Base/Renderer/Renderer.h"
#include "Base/Editer/Editer.h"
#include "Base/Object/Manaer/ObjectManager.h"

class GameManager
{
public:
	enum GameStateNo {
		PLAY,
		CLEAR,
		GameStateMax
	};

	void Run();

private:

	//Base
	MyEngine* myEngine = nullptr;
	WinApp* winApp = nullptr;
	DirectXCommon* directX = nullptr;
	TextureManager* textureManager = nullptr;

	std::unique_ptr<Renderer> renderer_ = nullptr;

	ImGuiManager* imGuiManager = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	Light* light = nullptr;

	Editer* editer = nullptr;

	ObjectManager* objectManager = nullptr;

	MSG msg{};
	//State
	std::unique_ptr<GameState>state[GameStateMax];
	Vector4 ClearColor{ 0.1f,0.25f,0.5f,1.0f };

	int currentSceneNum_;
	int prevSceneNum_;

	void Initialize();
	void Gameloop();
	void Release();
};


