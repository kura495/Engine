#pragma once
#include "VertexData.h"
#include "Material.h"
#include "Transform.h"
#include <vector>

struct Node {
	TransformQua transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node>children;
};

struct Joint {
	TransformQua transform;			// Transform情報
	Matrix4x4 localMatrix;			// localMatrix
	Matrix4x4 skeletonSpaceMatrix;	// skeletonSpaceでの変換行列
	std::string name;				// 名前
	std::vector<int32_t> children;	// 子JointのIndexのリスト。いなければ空
	int32_t index;					// 自身のIndex
	std::optional<int32_t>parent;	// 親jointのIndex。いなければnull
};

struct Skeleton {
	int32_t root;							// RootJointのIndex
	std::map<std::string, int32_t> jointMap;//Joint名とIndexとの辞書
	std::vector<Joint> joints;				//所属しているJoint
};

struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};
// 影響を受けるJointの数
const uint32_t kNumMaxInfluence = 4;
struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};

struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix;
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;
};

struct SkinCluster {
	std::vector<Matrix4x4> inverseBindPoseMatrices;
	Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPaette;
	DESCRIPTERHANDLE paletteSRVHandle;
};

struct ModelData {
	std::map<std::string, JointWeightData> skinClusterData;
	std::vector<VertexData> vertices;
	std::vector<int32_t> indices;
	MaterialData material;
	Node rootNode;
	int TextureIndex = 0;
};