#pragma once

#include "Common/DirectX/DirectXCommon.h"
#include "Manager/PSO/PSOManager.h"
#include "PipelineState/Standard/Standard.h"

class Renderer
{
public:
	static Renderer* GetInstance();
	void Initalize();
	//void Update();
	void Draw(PipelineType Type);

private:
	Renderer() = default;
	~Renderer() = default;
	Renderer(const Renderer& obj) = delete;
	Renderer& operator=(const Renderer& obj) = delete;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>commandList = nullptr;
	std::unique_ptr<PSOManager> PSOManager_ = nullptr;
};
