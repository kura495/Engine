#pragma once
#include "Material.h"
#include "Transform.h"
#include "VertexData.h"
#include "Common/DirectX/DirectXCommon.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Manager/SRV/SRVManager.h"
#include "Texture/TextureManager.h"
#include "Light/Light.h"
using namespace Math;
class Line
{
public:
	Line() = default;
	~Line() = default;

	void Init();
	/// <summary>
	/// 線の頂点の更新
	/// </summary>
	/// <param name="vertices"></param>
	void UpdateVertexData(std::vector<Vector4>& vertices);
	void Draw(const WorldTransform& transform);
	void RendererDraw(WorldTransform& transform);
	/// <summary>
	/// 頂点情報の登録
	/// </summary>
	/// <param name="vertices"></param>
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
