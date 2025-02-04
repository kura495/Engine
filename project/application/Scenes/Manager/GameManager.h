#pragma once
//ゲームシーンやポストプロセスを管理するシーン//
#include"Common/WinApp/WinApp.h"
#include"Common/DirectX/DirectXCommon.h"
#include"Common/Engine/MyEngine.h"
#include"Object/Mesh/Mesh.h"
#include"Texture/TextureManager.h"
#include"Light/Light.h"
#include"ImGuiManager.h"
#include"Scenes/State/Title/GameTitleState.h"
#include"Scenes/State/Play/GamePlayState.h"
#include"Scenes/State/Clear/GameClearState.h"
#include"Scenes/State/GameOver/GameOverState.h"
#include"Scenes/State/IgameState.h"
#include"Audio.h"
#include"Input.h"
#include"GlobalVariables.h"
#include "Renderer/Renderer.h"
#include "Editer/Editer.h"
#include "Object/Manager/ObjectManager.h"
#include "PostProsess/PostProsess.h"
#include "PostProsess/Filter/Filter.h"
#include "PostProsess/GrayScale/PPGrayScale.h"
#include "PostProsess/Vinette/PPVinette.h"
#include "PostProsess/Normal/Normal.h"
#include "PostProsess/LuminanceBasedOutline/PPLuminanceBasedOutline.h"
#include "PostProsess/RGBtoHSV/PPRGBtoHSV.h"
#include "PostProsess/CubeMap/PPCubeMap.h"

class GameManager
{
public:

	void Run();

private:

	//Base
	MyEngine* myEngine = nullptr;
	WinApp* winApp = nullptr;
	DirectXCommon* directX = nullptr;
	TextureManager* textureManager = nullptr;

	Renderer* renderer_ = nullptr;

	ImGuiManager* imGuiManager = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	Light* light = nullptr;

	Editer* editer = nullptr;

	ObjectManager* objectManager = nullptr;

	MSG msg{};
	//State
	std::unique_ptr<IGameState>state[GameStateMax];
	Vector4 ClearColor{ 0.1f,0.25f,0.5f,1.0f };
	//ポストプロセス
	std::unique_ptr<PPNormal> renderTextrue;

	int currentSceneNum_;
	int prevSceneNum_;

	void Initialize();
	void Gameloop();
	void Release();
};


