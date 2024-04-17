#pragma once

#include "Common/DirectX/DirectXCommon.h"
#include "externals/DirectXTex/DirectXTex.h"

using namespace Microsoft::WRL;

class PostProsess
{
public:
	void Init();

private:
	ComPtr<ID3D12Resource> CreateRenderTextureResource(ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

	DirectXCommon* DirectX;

	// 結果が入る　S_OKなら成功
	HRESULT hr;

};

