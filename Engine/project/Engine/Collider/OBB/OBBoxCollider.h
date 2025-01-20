#pragma once
//ボックス型コライダー(OBB)//
#include "Collider/ICollider.h"
#include "Collider/ColliderModel/OBB/OBBColliderModel.h"

struct OBBData {
	Vector3 center;//中心点の座標
	Vector3 orientations[3];//XYZの各座標軸の傾きを表す方向ベクトル
	float size[3];//各軸方向の長さ
};

class OBBoxCollider : public ICollider
{
public:
	OBBoxCollider();
	~OBBoxCollider();

	void Init(WorldTransform* world);
	void CollisionUpdate()override;
	void CollisionDraw()override;

	void SetOrientations(Matrix4x4 Matrix);
	/// <summary>
	/// 指定軸番号の方向ベクトルを取得
	/// </summary>
	/// <param name="elem">x = 0, y = 1, z = 2 数字で軸を選択</param>
	/// <returns>Vector3</returns>
	Vector3 GetDirect(int elem) {
		return obb_.orientations[elem];
	};
	/// <summary>
	/// 指定軸方向の長さを取得
	/// </summary>
	/// <param name="elem">x = 0, y = 1, z = 2 数字で軸を選択</param>
	/// <returns>float</returns>
	float GetLen(int elem) {
		return obb_.size[elem];
	};
	/// <summary>
	/// 位置を取得
	/// </summary>
	/// <returns>Vector3</returns>
	Vector3 GetPos() {
		return obb_.center;
	};
	/// <summary>
	/// 形状の判別をする関数
	/// </summary>
	/// <returns>Shape</returns>
	Shape GetShape() override {
		return Shape::OBB;
	};

private:

	std::unique_ptr<OBBColliderModel> model_;

	OBBData obb_;

	bool IsPushByCollider_ = false;
};