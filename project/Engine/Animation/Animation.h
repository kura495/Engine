#pragma once
// アニメーションを再生するクラス//
// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "KeyFrame/Struct/KeyFrame_Struct.h"
#include"ModelData.h"
#include "Math/Matrix/MatrixCalc.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Object/Model/Model.h"
#include"GlobalTime.h"
using namespace Math;
class Animation {
public:
	void Init();
	/// <summary>
	/// アニメーション用のスケルトンやスキンクラスターの作成
	/// </summary>
	/// <param name="model">スケルトンとスキンクラスターの元にするモデル</param>
	/// <param name="UseDebugLine">デバッグ用表示を使うかどうか</param>
	void AnimeInit(Model& model, bool UseDebugLine);
	/// <summary>
	/// アニメーションの再生を最初からにリセットする
	/// </summary>
	void Reset() {
		animationTime_ = 0;
		isFin = false;
	};
	//アニメーションファイルの読み込
	
	/// <summary>
	/// NodeAnimationの読み込み
	/// </summary>
	/// <param name="directrypath">ディレクトリパス</param>
	/// <param name="filename">ファイルネーム</param>
	/// <returns>Animation*</returns>
	static Animation* LoadAnimationFile(const std::string& directrypath,const std::string& filename);
	/// <summary>
	/// アニメーション補間
	/// </summary>
	/// <param name="animeA">補間前のアニメーション</param>
	/// <param name="animeB">補間後のアニメーション</param>
	/// <param name="t">補間の値</param>
	void AnimationLerp(Animation* animeA, Animation* animeB, float t);
	/// <summary>
	/// アニメーションを再生
	/// </summary>
	/// <param name="LoopFlag">アニメーションをループさせるかどうかのフラグ</param>
	void PlayAnimation(bool LoopFlag = true);
	/// <summary>
	/// 指定したフレームのアニメーションを再生
	/// </summary>
	/// <param name="LoopFlag">アニメーションをループさせるかどうかのフラグ</param>
	/// <param name="FlemeSpeed">指定したフレームのアニメーションを再生する</param>
	void PlayAnimation(float FlemeSpeed,bool LoopFlag = true);

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

	/// <summary>
	/// ノードアニメーションの更新
	/// </summary>
	/// <param name="keyframes">キーフレーム</param>
	/// <param name="time">経過時間</param>
	/// <returns>Vector3</returns>
	static Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keyframes, float time);
	/// <summary>
	/// ノードアニメーションの更新
	/// </summary>
	/// <param name="keyframes">キーフレーム</param>
	/// <param name="time">経過時間</param>
	/// <returns>Quaternion</returns>
	static Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes, float time);
	/// <summary>
	/// Skeletonのjointをすべて更新
	/// </summary>
	void SkeletonUpdate();
	/// <summary>
	/// SkinClusterの更新
	/// </summary>
	void SkinClusterUpdate();
	/// <summary>
	/// モデルから取得したノードからスケルトンを作成
	/// </summary>
	/// <param name="rootNode">モデルデータの中のノード</param>
	void CreateSkeleton(const Node& rootNode);
	/// <summary>
	/// スキンクラスターを作成
	/// </summary>
	/// <param name="modelData">モデルデータを取得</param>
	void CreateSkinCluster(const ModelData& modelData);
	/// <summary>
	/// 最初のノードをさかのぼってノードを取得
	/// </summary>
	/// <param name="node"></param>
	/// <param name="parent"></param>
	/// <param name="joints"></param>
	/// <returns>int32_t</returns>
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	/// <summary>
	/// ノードアニメーションの更新処理
	/// </summary>
	/// <param name="animationTime">経過時間</param>
	void ApplyAnimation(float animationTime);
	/// <summary>
	/// ボーンのデバッグライン表示用バーテックスを作成
	/// </summary>
	/// <param name="parentIndex">ジョイントの番号</param>
	/// <param name="vertices">作成するバーテックスデータの入れ物</param>
	void CreateBoneLineVertices(int32_t parentIndex, std::vector<Vector4>& vertices);

	float animationTime_ = 1.0f / 60.0f;

	void UpdateLine();
	Line SkeletonLine;
	std::vector<Vector4> point;
	Skeleton skeleton;
	SkinCluster skinCluster;

	bool IsDebugLine = false;

	bool isFin = false;
};