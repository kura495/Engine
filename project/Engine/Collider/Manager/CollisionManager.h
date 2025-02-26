#pragma once
//当たり判定管理クラス//
#include "Collider/ICollider.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Collider/CollisionConfig.h"
#include <list>

class CollisionManager {
public:

	void Init();
	void Update();
	void Draw();
	/// <summary>
	/// マネージャークラスに当たり判定を追加
	/// </summary>
	/// <param name="collider">追加するCollider</param>
	static void AddCollider(ICollider* collider) { Colliders_.push_back(collider); }
	/// <summary>
	/// 保持しているコライダーの全消去
	/// </summary>
	void ClearCollider() {
		Colliders_.clear();
	}
	/// <summary>
	/// 保持しているコライダーすべての当たり判定の処理
	/// </summary>
	void CheckAllCollisions();

private:
	/// <summary>
	///　コライダー登録用
	/// </summary>
	static std::list<ICollider*> Colliders_;
	
	// 関数ポインタの型
	using CollisionFunction = std::function<bool(ICollider*, ICollider*)>;
	CollisionFunction checkCollisions_[ICollider::Shape::MaxValue][ICollider::Shape::MaxValue];	
	// Shape::Countの値がサイズ！
	//当たり判定用関数
	//void CheckCollisionCircle(Collider* colliderA, Collider* colliderB);
	bool CheckCollision(BoxCollider* colliderA, BoxCollider* colliderB);
	bool CheckCollision(OBBoxCollider* colliderA, OBBoxCollider* colliderB);
	//void CheckCollitionOBBox(OBBoxCollider* colliderA, OBBoxCollider* colliderB);

	/// <summary>
	/// 分離軸に投影された軸成分から投影線分長を算出
	/// </summary>
	/// <param name="Sep">分離軸</param>
	/// <param name="e1">方向ベクトル</param>
	/// <param name="e2">方向ベクトル</param>
	/// <param name="e3">方向ベクトル</param>
	/// <returns>float</returns>
	float LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);
	/// <summary>
	/// OBB同士の判定でめり込んでいる分の計算
	/// </summary>
	/// <param name="obb1"></param>
	/// <param name="obb2"></param>
	/// <returns>Vector3</returns>
	Vector3 CalculateMTV(OBBoxCollider* obb1, OBBoxCollider* obb2);
	/// <summary>
	/// OBB同士の判定でめり込んでいる分の計算(軸ひとつづつ)
	/// </summary>
	/// <param name="obb1"></param>
	/// <param name="obb2"></param>
	/// <param name="axis"></param>
	/// <returns>float</returns>
	float CalculateOverlap(OBBoxCollider* obb1,OBBoxCollider* obb2,Vector3& axis);
};
