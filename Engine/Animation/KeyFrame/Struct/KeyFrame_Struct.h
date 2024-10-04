#pragma once
#include "Math/Math_Structs.h"
// キーフレーム構造体
template<typename tValue>
class KeyFrame{
public:
	float time;
	tValue value;
public:
};
using KeyFrameVector3 = KeyFrame<Vector3>;
using KeyFrameQuaternion = KeyFrame<Quaternion>;
// アニメーションカーブ構造体(ノードをまとめたもの)
template<typename tValue>
struct AnimationCurve {
	std::vector<KeyFrame<tValue>> keyFrames;
};
// ノード構造体
struct NodeAnimation {
	AnimationCurve<Vector3> translate;// 位置のデータ
	AnimationCurve<Quaternion> rotate;// 回転のデータ
	AnimationCurve<Vector3> scale;// 拡縮のデータ
};
