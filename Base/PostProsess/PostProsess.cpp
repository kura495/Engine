#include "PostProsess.h"

void PostProsess::Init()
{
	DirectX = DirectXCommon::GetInstance();
	const Vector4 kRenderTargetClearValue{ 1.0f,0.0f,0.0f,1.0f };// わかりやすい赤に設定
	auto renderTextureResource = CreateRenderTextureResource(DirectX->GetDevice(), WinApp::kClientWidth,WinApp::kClientHeight,DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,kRenderTargetClearValue);
	DirectX->GetDevice()->CreateRenderTargetView(renderTextureResource.Get(), &DirectX->GetrtvDesc(), DirectX->GetrtvHandles());

	// SRVを作る
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;
	// SRVの生成
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = DirectX->GetsrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += DirectX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 100;
	DirectX->GetDevice()->CreateShaderResourceView(renderTextureResource.Get(),&renderTextureSrvDesc, handleCPU);

}

ComPtr<ID3D12Resource> PostProsess::CreateRenderTextureResource(ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor)
{
	//metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(width);
	resourceDesc.Height = UINT(height);
	// RenderTargetとして使う
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	
	//利用するheapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;// VRAM上に作る

	// クリアの色と設定
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	//Resourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,// 描画することを前提としたTextureなのでRenderTargetとして使う事から始める
		&clearValue,//Clear最適値
		IID_PPV_ARGS(&resource)//作成するResourceポインタへのポインタ
	);

	return resource;
}
