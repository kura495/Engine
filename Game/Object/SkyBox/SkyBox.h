#pragma once
#include "Common/DirectX/DirectXCommon.h"
#include "Texture/TextureManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Renderer/Renderer.h"
#include "ViewProj/ViewProjection.h"
#include "Transform.h"
#include "VertexData.h"
#include <wrl.h>
#include <numbers>

class SkyBox {
public:
	void Init();

	void Draw(const WorldTransform& transform, const uint32_t& TextureHandle);

private:

	DirectXCommon* directX_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	//頂点データ
	VertexData* vertexData = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
	//マテリアルデータ
	Material* materialData = nullptr;
	//インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = nullptr;
	//インデックスデータ
	uint32_t* indexData = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	void CreateResource();
	void MakeVertexBufferView();
	void MakeIndexBufferView();


};