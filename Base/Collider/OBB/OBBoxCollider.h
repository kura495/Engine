#pragma once

#include "Base/Collider/Collider.h"

struct OBBData {
	Vector3 center;//中心点の座標
	Vector3 orientations[3];//XYZの各座標軸の傾きを表す方向ベクトル
	Vector3 size;//中心から面までの距離
};

class OBBoxCollider : public ICollider
{
public:
	void Initalize();

	void SetSize(Vector3 size) {obb_.size = size;}
	void SetCenter(Vector3 CenterPoint) { obb_.center = CenterPoint; }
	void SetOrientations(Matrix4x4 Matrix);

	Shape GetShape() override {
		return Shape::OBB;
	};

private:

	OBBData obb_;



};