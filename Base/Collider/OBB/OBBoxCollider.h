#pragma once

#include "Base/Collider/ICollider.h"
#include "Base/Collider/ColliderModel/OBB/OBBColliderModel.h"

struct OBBData {
	Vector3 center;//中心点の座標
	Vector3 orientations[3];//XYZの各座標軸の傾きを表す方向ベクトル
	float size[3];//各軸方向の長さ
};

class OBBoxCollider : public ICollider
{
public:
	void Initialize();
	void CollisionUpdate()override;
	void CollisionDraw()override;

	void SetOrientations(Matrix4x4 Matrix);

	Vector3 GetDirect(int elem) {
		return obb_.orientations[elem];
	};// 指定軸番号の方向ベクトルを取得
	float GetLen(int elem) {
		return obb_.size[elem];
	};   // 指定軸方向の長さを取得
	Vector3 GetPos() {
		return obb_.center;
	};// 位置を取得

	Shape GetShape() override {
		return Shape::OBB;
	};

private:

	std::unique_ptr<OBBColliderModel> model_;

	OBBData obb_;
};