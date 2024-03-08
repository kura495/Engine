#pragma once

#include "Math/Vector/Vector3.h"
#include "WorldTransform/WorldTransform.h"
#include "Collider.h"

struct AABB {
	Vector3 min;//一番小さいポイント
	Vector3 max;//一番大きいポイント
};

class BoxCollider : public Collider
{
public:

	void Initialize();
	void Update();

	AABB GetAABB() const { return aabb_; }

private:


	/// <summary>
	/// 箱の大きさ
	/// </summary>
	AABB aabb_ = {};
};
