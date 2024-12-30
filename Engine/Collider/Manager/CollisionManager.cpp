#include "CollisionManager.h"

std::list<ICollider*> CollisionManager::Colliders_;

void CollisionManager::Init()
{
	Colliders_.clear();
	checkCollisions_[ICollider::Shape::Box][ICollider::Shape::Box] = [this](ICollider* colliderA, ICollider* colliderB)
		{return CheckCollision(dynamic_cast<BoxCollider*>(colliderA), dynamic_cast<BoxCollider*>(colliderB));};
	checkCollisions_[ICollider::Shape::OBB][ICollider::Shape::OBB] = [this](ICollider* colliderA, ICollider* colliderB)
		{return CheckCollision(dynamic_cast<OBBoxCollider*>(colliderA), dynamic_cast<OBBoxCollider*>(colliderB));};

}

void CollisionManager::Update()
{
	Colliders_.remove_if([this](ICollider* Icollider)
		{
			if (Icollider->IsDalete)
			{
				return true;
			}
			return false;
		});

	for (ICollider* collider : Colliders_) {
		collider->CollisionUpdate();
	}

	CheckAllCollisions();
}

void CollisionManager::Draw()
{
#ifdef _DEBUG
	for (ICollider* collider : Colliders_) {
		if (collider->IsDalete == false) {
			collider->CollisionDraw();
		}
	}
#endif
}

void CollisionManager::CheckAllCollisions() {

	std::list<ICollider*>::iterator BoxitrA = Colliders_.begin();
	for (; BoxitrA != Colliders_.end(); ++BoxitrA) {

		// イテレータAからコライダーを取得
		ICollider* colliderA = *BoxitrA;
		// colliderAを使うかどうか
		if (colliderA->IsUsing == false) {
			continue;
		}

		// イテレータBはイテレータAの次の要素から回す(重複を避ける)
		std::list<ICollider*>::iterator BoxitrB = BoxitrA;
		BoxitrB++;
		for (; BoxitrB != Colliders_.end(); ++BoxitrB) {
			ICollider* colliderB = *BoxitrB;

			// colliderBを使うかどうか
			if (colliderB->IsUsing == false) {
				continue;
			}
			// フィルタを見る
			if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask()) == 0 && (colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask()) == 0) {
					continue;
			}
			if (checkCollisions_[colliderA->GetShape()][colliderB->GetShape()](colliderA, colliderB)) {
				//当たった時の処理呼び出し
				colliderA->OnCollision(*colliderB);
				colliderB->OnCollision(*colliderA);
			}
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
bool CollisionManager::CheckCollision(BoxCollider * colliderA, BoxCollider * colliderB)
{
	// 判定対象AとBの座標
	AABBData posA, posB;
	posA = colliderA->GetAABB();
	posB = colliderB->GetAABB();

	if ((posA.min.x <= posB.max.x && posA.max.x >= posB.min.x) && //x軸
		(posA.min.y <= posB.max.y && posA.max.y >= posB.min.y) && //y軸
		(posA.min.z <= posB.max.z && posA.max.z >= posB.min.z)    //z軸
		) {
		return true;
	}
	return false;
}

bool CollisionManager::CheckCollision(OBBoxCollider* colliderA, OBBoxCollider* colliderB)
{
	// 方向ベクトルと方向ベクトルの長さ
	Vector3 NAe1 = colliderA->GetDirect(0), Ae1 = NAe1 * colliderA->GetLen(0);
	Vector3 NAe2 = colliderA->GetDirect(1), Ae2 = NAe2 * colliderA->GetLen(1);
	Vector3 NAe3 = colliderA->GetDirect(2), Ae3 = NAe3 * colliderA->GetLen(2);
	Vector3 NBe1 = colliderB->GetDirect(0), Be1 = NBe1 * colliderB->GetLen(0);
	Vector3 NBe2 = colliderB->GetDirect(1), Be2 = NBe2 * colliderB->GetLen(1);
	Vector3 NBe3 = colliderB->GetDirect(2), Be3 = NBe3 * colliderB->GetLen(2);
	// 中心の位置
	Vector3 Interval = colliderA->GetPos() - colliderB->GetPos();
	float rA;
	float rB;
	float L;
	// 分離軸 : Ae1
	rA = Ae1.Length();
	rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	L = fabs(Vector3::Dot(Interval, NAe1));
	if (L > rA + rB) return false; // 衝突していない
	// 分離軸 : Ae2
	rA = Ae2.Length();
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(Vector3::Dot(Interval, NAe2));
	if (L > rA + rB) return false; // 衝突していない
	// 分離軸 : Ae3
	rA = Ae3.Length();
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(Vector3::Dot(Interval, NAe3));
	if (L > rA + rB) return false; // 衝突していない
	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = Be1.Length();
	L = fabs(Vector3::Dot(Interval, NBe1));
	if (L > rA + rB) return false;
	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = Be2.Length();
	L = fabs(Vector3::Dot(Interval, NBe2));
	if (L > rA + rB) return false;
	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = Be3.Length();
	L = fabs(Vector3::Dot(Interval, NBe3));
	if (L > rA + rB) return false;

	// 分離軸 : C11
	Vector3 Cross;
	Cross = Vector3::Cross(NAe1, NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	Cross = Vector3::Cross(NAe1, NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	Cross = Vector3::Cross(NAe1, NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	Cross = Vector3::Cross(NAe2, NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	Cross = Vector3::Cross(NAe2, NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	Cross = Vector3::Cross(NAe2, NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	Cross = Vector3::Cross(NAe3, NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	Cross = Vector3::Cross(NAe3, NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	Cross = Vector3::Cross(NAe3, NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	colliderA->pushForce = CalculateMTV(colliderA, colliderB);
	colliderB->pushForce = colliderA->pushForce;
	// 分離平面が存在しないので「衝突している」
	return true;
}

float CollisionManager::LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	FLOAT r1 = fabs(Vector3::Dot(*Sep, *e1));
	FLOAT r2 = fabs(Vector3::Dot(*Sep, *e2));
	FLOAT r3 = e3 ? (fabs(Vector3::Dot(*Sep, *e3))) : 0;
	return r1 + r2 + r3;
}

Vector3 CollisionManager::CalculateMTV(OBBoxCollider* obb1, OBBoxCollider* obb2) {
	Vector3 axes[] = {
		obb1->GetDirect(0), obb1->GetDirect(1), obb1->GetDirect(2),
		obb2->GetDirect(0), obb2->GetDirect(1), obb2->GetDirect(2),
		Vector3::Cross(obb1->GetDirect(0), obb2->GetDirect(0)),
		Vector3::Cross(obb1->GetDirect(0), obb2->GetDirect(1)),
		Vector3::Cross(obb1->GetDirect(0), obb2->GetDirect(2)),
		Vector3::Cross(obb1->GetDirect(1), obb2->GetDirect(0)),
		Vector3::Cross(obb1->GetDirect(1), obb2->GetDirect(1)),
		Vector3::Cross(obb1->GetDirect(1), obb2->GetDirect(2)),
		Vector3::Cross(obb1->GetDirect(2), obb2->GetDirect(0)),
		Vector3::Cross(obb1->GetDirect(2), obb2->GetDirect(1)),
		Vector3::Cross(obb1->GetDirect(2), obb2->GetDirect(2))
	};

	float minOverlap = FLT_MAX;
	Vector3 mtvAxis;

	for (Vector3 axis : axes) {
		if (axis.Length() < FLT_EPSILON) continue; // 無効な軸はスキップ

		axis.Normalize();
		float overlap = CalculateOverlap(obb1, obb2, axis);

		if (overlap < 0) return Vector3(0, 0, 0); // 衝突なし
		if (overlap < minOverlap) {
			minOverlap = overlap;
			mtvAxis = axis;
		}
	}

	return mtvAxis * minOverlap;
}

float CollisionManager::CalculateOverlap(OBBoxCollider* obb1, OBBoxCollider* obb2,Vector3& axis) {
	// 軸の正規化
	Vector3 normalizedAxis = axis.Normalize();

	// OBB1 の中心と半径の計算
	float center1 = Vector3::Dot(obb1->GetCenter(), normalizedAxis);
	float radius1 =
		obb1->GetLen(0) * fabs(Vector3::Dot(obb1->GetDirect(0), normalizedAxis)) +
		obb1->GetLen(1) * fabs(Vector3::Dot(obb1->GetDirect(1), normalizedAxis)) +
		obb1->GetLen(2) * fabs(Vector3::Dot(obb1->GetDirect(2), normalizedAxis));

	// OBB2 の中心と半径の計算
	float center2 = Vector3::Dot(obb1->GetCenter(), normalizedAxis);
	float radius2 =
		obb2->GetLen(0) * fabs(Vector3::Dot(obb2->GetDirect(0), normalizedAxis)) +
		obb2->GetLen(1) * fabs(Vector3::Dot(obb2->GetDirect(1), normalizedAxis)) +
		obb2->GetLen(2) * fabs(Vector3::Dot(obb2->GetDirect(2), normalizedAxis));

	// 重なり量の計算
	float overlap = (radius1 + radius2) - fabs(center1 - center2);

	// 結果を返す
	return overlap;
}