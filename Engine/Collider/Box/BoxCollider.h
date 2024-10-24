#pragma once
//ボックス型コライダー(AABB)//
//TODO:未完成
#include "Math/Vector/Vector3.h"
#include "WorldTransform/WorldTransform.h"
#include "Collider/ICollider.h"
#include "Collider/ColliderModel/Box/BoxColliderModel.h"

struct AABBData {
	Vector3 min;//一番小さいポイント
	Vector3 max;//一番大きいポイント
};

class BoxCollider : public ICollider
{
public:

	void Init(WorldTransform* Parent);
	void CollisionUpdate()override;
	void CollisionDraw()override;

	AABBData GetAABB() const { return aabb_; }

	Shape GetShape() override {
		return Shape::Box;
	};

private:

	std::unique_ptr<BoxColliderModel> model_;
	/// <summary>
	/// 箱の大きさ
	/// </summary>
	AABBData aabb_ = {};
};
