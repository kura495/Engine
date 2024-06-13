#pragma once
// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "KeyFrame/Struct/KeyFrame_Struct.h"
#include "Base/Math/Vector/VectorCalc.h"
#include "ModelData.h"
#include "Base/Math/Matrix/MatrixCalc.h"
#include "Base/WorldTransform/WorldTransform.h"
#include "Base/ViewProj/ViewProjection.h"
#include "Object/Model/Model.h"

// アニメーション構造体
class Animation {
public:
	void Init();
	void AnimeInit(Model& model);

	static Animation* LoadAnimationFile(const std::string& directrypath,const std::string& filename);
	static Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keyframes,float time);
	static Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes,float time);

	void PlayAnimation();

	void DebugDraw(WorldTransform& world);

	SkinCluster& GetSkinCluster() {
		return skinCluster;
	};



	float duration; // アニメーション全体の尺(単位は秒)
	// NodeAnimationの集合　Node名で検索できるようにする
	std::map<std::string, NodeAnimation> nodeAnimations;
private:
	DirectXCommon* directX_ = nullptr;
	SRVManager* srvManager_;

	void SkeletonUpdate();
	void SkinClusterUpdate();
	void CreateSkeleton(const Node& rootNode);
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	void ApplyAnimation(float animationTime);
	void CreateSkinCluster(const ModelData& modelData);
	void CreateBoneLineVertices(int32_t parentIndex, std::vector<Vector4>& vertices);

	float animationTime_ = 1.0f / 60.0f;

	void UpdateLine();
	Line SkeletonLine;
	std::vector<Vector4> point;
	Skeleton skeleton;
	SkinCluster skinCluster;
};