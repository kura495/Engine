#pragma once

#include "Light.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Math/MatrixCalc.h"
#include "ModelData.h"

#include "Utility/BoxCollider.h"

struct Line {
	Vector4 top;
	Vector4 bottom;
};

class Plane : public BoxCollider
{
public:
	void Initalize(const std::string filePath);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision(const Collider*collider)override;

private:
	ModelData modelData;
	WorldTransform world_;

	DirectXCommon* directX_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	Light* light_ = nullptr;
	//どのライトを使うか
	int32_t lightFlag = Lighting::NotDo;

	Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource = nullptr;
	VertexData* vertexData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
	Material* materialData = nullptr;

	Line left = { .top = {-1.0f,0.0f,1.0f,1.0f},.bottom = {-1.0f,0.0f,-1.0f,1.0f} };
	Line right = { .top = {1.0f,0.0f,1.0f,1.0f},.bottom = {1.0f,0.0f,-1.0f,1.0f} };

	void CreateResources();
	void CreateSRV();

	void ImGui();
	int selectNumber_ = 0;
};