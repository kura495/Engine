#pragma once
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
#include "Base/Math/Matrix/MatrixCalc.h"

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
	void SkeletonUpdate(Skeleton& skeleton);
	void SkinClusterUpdate(SkinCluster& skinCluster, Skeleton& skeleton);
	void Draw(const WorldTransform& transform,const ViewProjection& viewProjection);
	void SkinDraw(const WorldTransform& transform,const ViewProjection& viewProjection,const SkinCluster& skinCluster);

	static Model* CreateModelFromObj(const std::string& directoryPath, const std::string& filename);
	ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);
	void SetLightMode(Lighting number) { lightFlag = number; };
	//skeletonAnimation
	Skeleton CreateSkeleton(const Node& rootNode);
	int32_t CreateJoint(const Node& node,const std::optional<int32_t>& parent,std::vector<Joint>& joints);
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);
	SkinCluster CreateSkinCluster(const Skeleton& skeleton, const ModelData& modelData);

	std::vector<Model*> SkeletonDebugInit(Skeleton& skeleton, std::vector<WorldTransform>& worlds);
	void SkeletonDebugUpdate(Skeleton& skeleton, const Animation& animation, float animationTime, std::vector<WorldTransform>& worlds);

	ModelData& GetModelData() { return modelData_; }

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
	//色
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	Node ReadNode(aiNode* node);

};