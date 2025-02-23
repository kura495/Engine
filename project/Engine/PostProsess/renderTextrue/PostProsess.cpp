#include "PostProsess.h"

void PostProsess::Init()
{
	DirectX = DirectXCommon::GetInstance();
	sRVManager_ = SRVManager::GetInstance();

	addInit();
}

void PostProsess::addInit()
{
}

void PostProsess::Draw()
{

	DirectX->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//テクスチャ
	DirectX->GetcommandList()->SetGraphicsRootDescriptorTable(0, SRVhandle.GPU);
	//色
	DirectX->GetcommandList()->SetGraphicsRootConstantBufferView(1, materialResource.Get()->GetGPUVirtualAddress());

	addDraw();

	DirectX->GetcommandList()->DrawInstanced(3, 1, 0, 0);
	//画像からレンダーターゲット
	PostCopy();
}

void PostProsess::addDraw()
{
}

void PostProsess::Create(int Index)
{
	CreateRTV(Index);
	CreateSRV();
	CreateDepth();
	CreateViewport();
	CreateScissor();
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(renderTextureResource.Get(), &renderTextureSrvDesc, SRVhandle.CPU);
	materialResource = DirectX->CreateBufferResource(sizeof(Vector3));
	materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	CreateBuffer();

	materialData->x = 0.0f;
}
void PostProsess::CreateBuffer()
{
}

void PostProsess::PreDraw()
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	DirectX->GetcommandList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
	//指定した色で画面全体をクリア
	float clearColor[] = { 1.0f,0.0f,0.0f,1.0f };//いつもの青っぽいやつ
	DirectX->GetcommandList()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	//指定した深度で画面全体をクリアする
	DirectX->GetcommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { DirectX->GetsrvDescriptorHeap().Get()};
	DirectX->GetcommandList()->SetDescriptorHeaps(1, descriptorHeaps);
	DirectX->GetcommandList()->RSSetViewports(1, &viewport);
	DirectX->GetcommandList()->RSSetScissorRects(1, &scissorRect);
}

void PostProsess::PostDraw()
{
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを貼る対象のリソース
	barrier.Transition.pResource = renderTextureResource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//TransitionBarrierを張る
	DirectX->GetcommandList()->ResourceBarrier(1, &barrier);
}

void PostProsess::PostCopy()
{	
	//RenderTargetからPresentにする
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	DirectX->GetcommandList()->ResourceBarrier(1, &barrier);
}

ComPtr<ID3D12Resource> PostProsess::CreateRenderTextureResource(ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	// HEAPを作る
	D3D12_HEAP_PROPERTIES HeapProperties{};
	HeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	
	//Resource設定
	D3D12_RESOURCE_DESC ResourceDesc{};
	ResourceDesc.Width = width;//Textureの幅
	ResourceDesc.Height = height;//Textureの高さ
	ResourceDesc.MipLevels = 1;//mipmapの数
	ResourceDesc.DepthOrArraySize = 1;//奥行きor配列Textureの配列数
	ResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//DepthStencilとして利用可能なフォーマット
	ResourceDesc.SampleDesc.Count = 1;//サンプリングカウント　1固定
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//2D
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;//DepthStencilとして使う通知

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

void PostProsess::CreateRTV(int Index)
{
	//RTVを作る
	const Vector4 kRenderTargetClearValue{ 1.0f,0.0f,0.0f,1.0f };
	renderTextureResource = CreateRenderTextureResource(DirectX->GetDevice(), WinApp::kClientWidth, WinApp::kClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	uint32_t descriptorSizeRTV = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	rtvHandle.ptr = DirectX->GetrtvHandles().ptr;
	for (int i = 0; i < Index; i++) {
		rtvHandle.ptr += descriptorSizeRTV;
	}
	//DirectXCommon::MakeDescriptorHeapの//rtvディスクリプタヒープの作成のNumDescriptorsを変える
	DirectX->GetDevice()->CreateRenderTargetView(renderTextureResource.Get(), &rtvDesc, rtvHandle);
}

void PostProsess::CreateSRV()
{
	//SRVを作る
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;

	SRVhandle = sRVManager_->GetDescriptorHandle();
	SRVhandle.CPU.ptr += sRVManager_->descriptorSizeSRV;
	SRVhandle.GPU.ptr += sRVManager_->descriptorSizeSRV;
}

void PostProsess::CreateDepth()
{
	//DSVDescriptorHeap
	depthStencilResource = DirectX->CreateDepthStencilTextureResource(WinApp::kClientWidth, WinApp::kClientHeight);
	//DSV用のヒープでディスクリプタの数は1　DSVはShader内で触るものではないのでShaderVisibleはfalse
	dsvDescriptorHeap = DirectX->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//Format 基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;//2dTexture
	//DSVHeapの先頭にDSVを作る
	DirectX->GetDevice()->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

void PostProsess::CreateViewport()
{
	viewport.Width = (float)WinApp::kClientWidth;
	viewport.Height = (float)WinApp::kClientHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void PostProsess::CreateScissor()
{
	scissorRect.left = 0;
	scissorRect.right = WinApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kClientHeight;
}
