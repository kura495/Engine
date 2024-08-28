#pragma once
#include "Common/DirectX/DirectXCommon.h"
#include "Texture/TextureManager.h"
#include "Transform.h"
#include "VertexData.h"
#include "Material.h"
#include "Math/Matrix/MatrixCalc.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include <wrl.h>
#include "Light/Light.h"


class Sprite
{
public:
	void Initialize(const Vector2& LeftTop, const Vector2& LeftBottom, const Vector2& RightTop, const Vector2& RightBottom);
	void Initialize(const Vector2& anchorPoint, const Vector2& TextureSize);
	void RendererDraw(WorldTransform& transform);
	void Draw(const WorldTransform& transform);
	void ImGui(const char* Title);
	void SetColor(const Vector4& color);
	//テクスチャのハンドル
	uint32_t TextureHandle = 0;
private:
	DirectXCommon* directX_ = nullptr;
	TextureManager* textureManager_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	VertexData* vertexData = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
	Material* materialData = nullptr;
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	//Index用
	Microsoft::WRL::ComPtr<ID3D12Resource>indexResource = nullptr;
	uint32_t* indexData = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	Transform uvTranform{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};


	ViewProjection viewProjection_;

	void MakeVertexBufferView();
	void MakeIndexBufferView();

	Light* light_ = nullptr;
};
