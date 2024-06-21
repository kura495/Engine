#include "CollisionManager.h"

void CollisionManager::Init()
{
	checkCollisions_[ICollider::Shape::Box][ICollider::Shape::Box] = [this](ICollider* colliderA, ICollider* colliderB)
		{
			return CheckCollision(dynamic_cast<BoxCollider*>(colliderA), dynamic_cast<BoxCollider*>(colliderB));
		};

}

void CollisionManager::Update()
{
	for (ICollider* collider : Colliders_) {
		collider->CollisionUpdate();
	}
	//TODO : ClearColliderの関係で仮置き
	for (ICollider* collider : Colliders_) {
		collider->CollisionDraw();
	}

	CheckAllCollisions();
	ClearCollider();
}

void CollisionManager::Draw()
{
	//for (ICollider* collider : Colliders_) {
	//	collider->CollisionDraw();
	//}
}

void CollisionManager::CheckAllCollisions() {

	std::list<ICollider*>::iterator BoxitrA = Colliders_.begin();
	for (; BoxitrA != Colliders_.end(); ++BoxitrA) {
		// イテレータAからコライダーを取得
		ICollider* colliderA = *BoxitrA;
		// イテレータBはイテレータAの次の要素から回す(重複を避ける)
		std::list<ICollider*>::iterator BoxitrB = BoxitrA;
		BoxitrB++;
		for (; BoxitrB != Colliders_.end(); ++BoxitrB) {
			ICollider* colliderB = *BoxitrB;
			checkCollisions_[colliderA->GetShape()][colliderB->GetShape()](colliderA, colliderB);
		}
	}
}
//void CollisionManager::CheckCollisionCircle(Collider * colliderA, Collider * colliderB) {
//	// 判定対象AとBの座標
//	Vector3 posA = { 1.0f,1.0f,1.0f }, posB = {1.0f, 1.0f,1.0f};
//	//posA = colliderA->GetWorldPosition();
//	//posB = colliderB->GetWorldPosition();
//	float Length =(float)sqrt(
//	    (posB.x - posA.x) * (posB.x - posA.x) +
//		(posB.y - posA.y) * (posB.y - posA.y) +
//	    (posB.z - posA.z) * (posB.z - posA.z));
//	// コライダーのフィルターの値でビット演算
//	if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask()) == 0 && (colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask()) == 0) {
//		return;
//	}
//	else if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask()) == 0) {
//		if (Length <= colliderA->GetRadius() + colliderB->GetRadius()) {
//			// コライダーAの衝突時コールバック
//			colliderA->OnCollision(colliderB->GetcollitionAttribute());
//		}
//	}
//	else if ((colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask()) == 0) {
//		if (Length <= colliderA->GetRadius() + colliderB->GetRadius()) {
//			// コライダーBの衝突時コールバック
//			colliderB->OnCollision(colliderA->GetcollitionAttribute());
//		}
//	}
//	else {
//		if (Length <= colliderA->GetRadius() + colliderB->GetRadius()) {
//			// コライダーAの衝突時コールバック
//			colliderA->OnCollision(colliderB->GetcollitionAttribute());
//			// コライダーBの衝突時コールバック
//			colliderB->OnCollision(colliderA->GetcollitionAttribute());
//		}
//	}
//}
//void CollisionManager::CheckCollitionOBBox(OBBoxCollider* colliderA, OBBoxCollider* colliderB)
//{
//	
//}
void CollisionManager::CheckCollision(BoxCollider * colliderA, BoxCollider * colliderB)
{
	// 判定対象AとBの座標
	AABBData posA, posB;
	posA = colliderA->GetAABB();
	posB = colliderB->GetAABB();

	// コライダーのフィルターの値でビット演算
	if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask()) == 0 && (colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask()) == 0) {
		return;
	}

	if ((posA.min.x <= posB.max.x && posA.max.x >= posB.min.x) && //x軸
		(posA.min.y <= posB.max.y && posA.max.y >= posB.min.y) && //y軸
		(posA.min.z <= posB.max.z && posA.max.z >= posB.min.z)    //z軸
		) {
		if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask()) == 0) {
			// コライダーAの衝突時コールバック
			colliderA->OnCollision(colliderB);
		}
		else if ((colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask()) == 0) {
			// コライダーBの衝突時コールバック
			colliderB->OnCollision(colliderA);
		}
		else {
			// コライダーAの衝突時コールバック
			colliderA->OnCollision(colliderB);
			// コライダーBの衝突時コールバック
			colliderB->OnCollision(colliderA);
		}
	}

}
