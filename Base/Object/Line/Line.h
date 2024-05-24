#pragma once
#include "Material.h"
#include "Transform.h"
#include "VertexData.h"
#include "Base/Common/DirectX/DirectXCommon.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Base/Manager/SRV/SRVManager.h"
#include "Texture/TextureManager.h"
#include "Light/Light.h"

class Line
{
public:
	Line() = default;
	~Line() = default;

	void Init();
	void UpdateVertexData(std::vector<Vector4>& vertices);
	void Draw(const WorldTransform& transform);
	void RendererDraw(WorldTransform& transform);

	void SetVertexData(std::vector<Vector4>& vertices);
	void CreateBuffer();

private:
	DirectXCommon* directX_ = nullptr;
	SRVManager* srvManager_ = nullptr;

	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	//頂点データ
	std::vector<VertexData> debugVertices_;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
	//マテリアルデータ
	Material* materialData = nullptr;
	//色
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
};
