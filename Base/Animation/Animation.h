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


// アニメーション構造体
class Animation {
public:
	void Init();

	static Animation LoadAnimationFile(const std::string& directrypath,const std::string& filename);
	static Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keyframes,float time);
	static Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes,float time);

	void SkeletonUpdate(Skeleton& skeleton);
	void SkinClusterUpdate(SkinCluster& skinCluster, Skeleton& skeleton);
	Skeleton CreateSkeleton(const Node& rootNode);
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);
	SkinCluster CreateSkinCluster(const Skeleton& skeleton, const ModelData& modelData);

	void CreateBoneLineVertices(const Skeleton& skeleton, int32_t parentIndex, std::vector<Vector4>& vertices);

	float duration; // アニメーション全体の尺(単位は秒)
	// NodeAnimationの集合　Node名で検索できるようにする
	std::map<std::string, NodeAnimation> nodeAnimations;
private:
	DirectXCommon* directX_ = nullptr;
	SRVManager* srvManager_;

	std::vector<Line*> SkeletonLine;
};