#pragma once
//モデルオブジェクト//
#include "Material.h"
#include "Transform.h"
#include "VertexData.h"
#include "ModelData.h"
#include "Texture/TextureManager.h"
#include "Light/Light.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"
#include "Math/Matrix/MatrixCalc.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Animation/Animation.h"
#include "Math/Matrix/MatrixCalc.h"

#include <d3d12.h>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <wrl.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	void Initialize(const std::string& directoryPath, const std::string& filename);
	void Draw(const WorldTransform& transform, int DDSTexHandle);
	void WireFlameDraw(const WorldTransform& transform);
	void SkinDraw(const WorldTransform& transform,const SkinCluster& skinCluster);
	void SkinDissolveDraw(const WorldTransform& transform,const SkinCluster& skinCluster,int DissolveTex,float DeissolveValue);


	void RendererDraw(WorldTransform& transform);
	void RendererSkinDraw(WorldTransform& transform, SkinCluster& skinCluster);
	void RendererSkinDissolveDraw(WorldTransform& transform, SkinCluster& skinCluster, float DissolveValue);
	void WireFrameDraw(WorldTransform& transform);

	static Model* CreateModelFromObj(const std::string& directoryPath, const std::string& filename);
	ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);
	void SetLightMode(Lighting number) { lightFlag = number; };

	ModelData& GetModelData() { return modelData_; }
	Material* GetMaterial() { return materialData; }

	//色
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
private:

	DirectXCommon* directX_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	Light* light_ = nullptr;
	SRVManager* srvManager_;
	//どのライトを使うか
	int32_t lightFlag = Lighting::NotDo;

	ModelData modelData_;
	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	//頂点データ
	VertexData* vertexData = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	//Indexリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = nullptr;
	//Indexデータ
	uint32_t* mappedIndex = nullptr;
	//Indexバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
	//マテリアルデータ
	Material* materialData = nullptr;
	//ディゾルブのおおきさ
	Microsoft::WRL::ComPtr<ID3D12Resource> DissolveResource = nullptr;
	Vector4* dissolveValue_ = nullptr;
	
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	Node ReadNode(aiNode* node);

};