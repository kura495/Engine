#pragma once

#include "Math/Vector/Vector3.h"
#include "WorldTransform/WorldTransform.h"
#include "Base/Collider/Collider.h"

struct AABBData {
	Vector3 min;//一番小さいポイント
	Vector3 max;//一番大きいポイント
};

class BoxCollider : public ICollider
{
public:

	void Initialize();
	void Update();

	AABBData GetAABB() const { return aabb_; }

	Shape GetShape() override {
		return Shape::Box;
	};

private:


	/// <summary>
	/// 箱の大きさ
	/// </summary>
	AABBData aabb_ = {};
};
