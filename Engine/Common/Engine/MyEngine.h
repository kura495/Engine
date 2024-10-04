#pragma once

//Base
#include "Common/WinApp/WinApp.h"
#include "Common/DirectX/DirectXCommon.h"
#include "Texture/TextureManager.h"
//other
#include <numbers>
#include <fstream>

class MyEngine
{
public:

	static MyEngine* GetInstance();
	void Initialize();

private:

	static const int kMaxTexture=10;

	//シングルトン
	MyEngine() = default;
	~MyEngine() = default;
	MyEngine(const MyEngine& obj) = delete;
	MyEngine& operator=(const MyEngine& obj) = delete;

	bool IsusedTextureIndex[kMaxTexture];

	HRESULT hr;
	WinApp* winApp_ = nullptr;
	DirectXCommon* directX_ = nullptr;
	TextureManager* textureManager_ = nullptr;
};