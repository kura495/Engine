#pragma once

#include "Base/Math/Struct/Vector3.h"
#include "Base/WorldTransform.h"
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

	///worldの親を設定
	void SetWorld(WorldTransform* Parent) { center_ = Parent; }
	
	void SetSize(Vector3 range) { range_ = range; }
	AABB GetSize() { return aabb_; }

private:
	/// <summary>
	/// x,y,zそれぞれの幅
	/// </summary>
	Vector3 range_;
	/// <summary>
	/// 原点
	/// </summary>
	WorldTransform* center_;
	/// <summary>
	/// 箱の大きさ
	/// </summary>
	AABB aabb_ = {};
};
