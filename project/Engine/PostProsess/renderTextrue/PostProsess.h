#pragma once
/*ポストプロセスクラス*/
#include "Common/DirectX/DirectXCommon.h"
#include "Manager/SRV/SRVManager.h"
#include "Object/Player/Player.h"
#include "externals/DirectXTex/DirectXTex.h"

using namespace Microsoft::WRL;

class PostProsess
{
public:
	void Init();
	/// <summary>
	/// 追加の初期化処理を行う場合にオーバーライド
	/// </summary>
	virtual void addInit();
	void Draw();
	/// <summary>
	/// 追加の描画処理を行う場合にオーバーライド
	/// </summary>
	virtual void addDraw();
	///レンダーターゲットからシェーダーリソースに変更
	void PostDraw();
	/// <summary>
	/// RTVやSRVなど、必要な物を作成
	/// </summary>
	/// <param name="Index">RTVの個数を代入</param>
	void Create(int Index);
	virtual void CreateBuffer();
	virtual void Update() {};
	void PreDraw();


protected:
	Vector4* materialData = nullptr;

protected:

	///シェーダーリソースからレンダーターゲットに変更
	void PostCopy();

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

