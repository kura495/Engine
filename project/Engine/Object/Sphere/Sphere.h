﻿#pragma once
//球体オブジェクト//
#include "Common/DirectX/DirectXCommon.h"
#include "Texture/TextureManager.h"
#include "Light/Light.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Transform.h"
#include "VertexData.h"
#include "Material.h"
#include "Math/Matrix/MatrixCalc.h"
#include <wrl.h>
#include <numbers>

class Sphere
{
public:

	void Initialize();
	void Draw(const WorldTransform& transform, const ViewProjection& viewProjection, const uint32_t& TextureHandle);

#pragma region
	void SetShiniess(float value) { materialData->shiniess = value; }
#pragma endregion Setter
private:
	DirectXCommon* directX_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	Light* light_ = nullptr;
	//分割数
	const int kSubdivision = 16;
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
	//色
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = nullptr;
	//インデックスデータ
	uint32_t* indexData = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	void MakeVertexBufferView();
	void MakeIndexBufferView();
	
};
