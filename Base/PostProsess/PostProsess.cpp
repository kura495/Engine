#include "PostProsess.h"

void PostProsess::Init()
{
	DirectX = DirectXCommon::GetInstance();
	sRVManager_ = SRVManager::GetInstance();

}

void PostProsess::CreateRTV()
{
	const Vector4 kRenderTargetClearValue{ 1.0f,0.0f,0.0f,1.0f };
	auto renderTextureResource = CreateRenderTextureResource(DirectX->GetDevice(),WinApp::kClientWidth,WinApp::kClientHeight,DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,kRenderTargetClearValue);
	//DirectX->GetDevice()->CreateRenderTargetView(renderTextureResource.Get(),&rtvDesc,);
}

ComPtr<ID3D12Resource> PostProsess::CreateRenderTextureResource(ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	// HEAPを作る
	D3D12_HEAP_PROPERTIES HeapProperties{};
	HeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC ResourceDesc{};
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.Width = width;
	ResourceDesc.Height = height;
	
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	//頂点リソースを作る
	hr = device.Get()->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_RENDER_TARGET, &clearValue, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));

	return Resource;
}
