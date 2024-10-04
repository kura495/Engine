#pragma once
#include "Common/DirectX/DirectXCommon.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "Manager/SRV/SRVManager.h"

#include <wrl.h>
#include <array>

class TextureManager
{
public:
	// デスクリプターの数
	static const size_t kMaxTexture = 256;

	/// <summary>
	/// テクスチャ
	/// </summary>
	struct Texture {
		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;
		// シェーダリソースビューのハンドル(CPU + GPU)
		DESCRIPTERHANDLE textureSrvHandle;
		// 名前
		std::string name;
		// 使っているかどうか
		bool IsUsed = false;
	};

	static TextureManager* GetInstance();
	void Initialize(DirectXCommon* directX);
	uint32_t LoadTexture(const std::string& filePath);
	/// <summary>
	/// GPUHandle情報取得
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	const CD3DX12_GPU_DESCRIPTOR_HANDLE  GetGPUHandle(uint32_t textureHandle);
private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager& obj) = delete;
	TextureManager& operator=(const TextureManager& obj) = delete;

	HRESULT hr;

	const std::string directoryPath = "resources/";
	
	bool IsusedTextureIndex[kMaxTexture];
	DirectXCommon* directX_ = nullptr;
	SRVManager* sRVManager_ = nullptr;
	// テクスチャコンテナ
	std::array<Texture, kMaxTexture> textures_;
	//中間リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource_[kMaxTexture];
	
	DirectX::ScratchImage ImageFileOpen(const std::string& filePath);
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device>device, const DirectX::TexMetadata& metadata);
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);

#pragma region descriptorHeap

	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;
#pragma endregion デスクリプタ
};

