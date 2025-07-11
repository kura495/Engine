#pragma once
#include "../InterFace/Iwork.h"
#include "Vector/Vector3.h"
#include "WorldTransform/WorldTransform.h"

//ベクトル方向に向ける処理クラス
class WorkLockAt : public Iwork {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="axis">回転の中心軸の位置</param>
	/// <param name="target">向きたい物</param>
	void Init(const Vector3& axis, const WorldTransform& target);
	/// <summary>
	/// ターゲットの方向に線形補間をしながら指定した軸を回転
	/// </summary>
	/// <param name="cameraRatVector">回転させるオイラー角</param>
	void Update(float& cameraRatVector);
private:
	//回転
	float lockAtRat = 0.0f;
};