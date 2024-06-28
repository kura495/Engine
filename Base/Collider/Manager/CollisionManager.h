#pragma once
#include "Base/Collider/ICollider.h"
#include "Base/Collider/Box/BoxCollider.h"
#include "Base/Collider/OBB/OBBoxCollider.h"
#include "Base/Collider/CollisionConfig.h"
#include <list>

class CollisionManager {
public:

	void Init();
	void Update();
	void Draw();

	void AddCollider(ICollider* collider) { Colliders_.push_back(collider); }
	void ClearCollider() {
		Colliders_.clear();
	}
	void CheckAllCollisions();

private:
	/// <summary>
	///　コライダー登録用
	/// </summary>
	std::list<ICollider*> Colliders_;
	
	// 関数ポインタの型
	using CollisionFunction = std::function<bool(ICollider*, ICollider*)>;
	CollisionFunction checkCollisions_[ICollider::Shape::MaxValue][ICollider::Shape::MaxValue];	// Shape::Countの値がサイズだよ！
	//判定用関数
	//void CheckCollisionCircle(Collider* colliderA, Collider* colliderB);
	bool CheckCollision(BoxCollider* colliderA, BoxCollider* colliderB);
	bool CheckCollision(OBBoxCollider* colliderA, OBBoxCollider* colliderB);
	//void CheckCollitionOBBox(OBBoxCollider* colliderA, OBBoxCollider* colliderB);

	float LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);
};
