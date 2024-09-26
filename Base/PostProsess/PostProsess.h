#pragma once

#include "Common/DirectX/DirectXCommon.h"
#include "Base/Manager/SRV/SRVManager.h"
#include "Game/Object/Player/Player.h"
#include "externals/DirectXTex/DirectXTex.h"

using namespace Microsoft::WRL;

class PostProsess
{
public:
	void Init();
	virtual void addInit();
	void Draw();
	virtual void addDraw();
	void Create(int Index);
	virtual void CreateBuffer();
	virtual void Update() = 0;
	void PreDraw();
	void PreCopy();
	void PostCopy();

protected:
	Vector3* materialData = nullptr;

protected:
	ComPtr<ID3D12Resource> CreateRenderTextureResource(ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

	void CreateRTV(int Index);
	void CreateSRV();
	void CreateDepth();
	void CreateViewport();
	void CreateScissor();

	DirectXCommon* DirectX = nullptr;
	SRVManager* sRVManager_ = nullptr;

	// 結果が入る　S_OKなら成功
	HRESULT hr;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	// RTV
	ComPtr<ID3D12Resource> renderTextureResource;
	// SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	DESCRIPTERHANDLE SRVhandle;
	// DSV関連
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>dsvDescriptorHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource>depthStencilResource = nullptr;
	//
	D3D12_VIEWPORT viewport{};
	//
	D3D12_RECT scissorRect{};
	//
	D3D12_RESOURCE_BARRIER barrier{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;

	Vector4 originalColor;
};

