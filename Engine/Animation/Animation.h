#pragma once
// アニメーションを再生するクラス//
// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "KeyFrame/Struct/KeyFrame_Struct.h"
#include "Math/Vector/VectorCalc.h"
#include "ModelData.h"
#include "Math/Matrix/MatrixCalc.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Object/Model/Model.h"
using namespace Math;
class Animation {
public:
	void Init();
	void AnimeInit(Model& model, bool UseDebugLine);
	void Reset() {
		animationTime_ = 0;
	};
	//アニメーションファイルの読み込み
	static Animation* LoadAnimationFile(const std::string& directrypath,const std::string& filename);
	static Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keyframes,float time);
	static Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes,float time);
	/// <summary>
	/// アニメーション補間
	/// </summary>
	/// <param name="animeA">補間前のアニメーション</param>
	/// <param name="animeB">補間後のアニメーション</param>
	/// <param name="t">補間の値</param>
	void AnimationLerp(Animation* animeA, Animation* animeB, float t);

	void PlayAnimation();

	/// <summary>
	/// スケルトンを描画する
	/// </summary>
	/// <param name="world">描画に使用するworldTransform</param>
	void DebugDraw(WorldTransform& world);
#pragma region
	SkinCluster& GetSkinCluster() {
		return skinCluster;
	};
	Skeleton& GetSkeleton() {
		return skeleton;
	};
#pragma endregion Getter Setter


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

	bool IsDebugLine = false;
};