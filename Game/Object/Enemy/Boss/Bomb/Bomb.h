#pragma once
#include "Object/IObject.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include <algorithm>

class Bomb : public IObject
{
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;
	/// <summary>
	/// 爆弾を投げる時に呼び出す関数
	/// </summary>
	/// <param name="targetVector">ターゲットのワールド座標</param>
	void ThrowBomb(Vector3 targetVector) {
		isThrowFlag = true;
		//方向を決める
		Vector3 playerToBomb = targetVector - world_.transform.translate;
		//速さの定数を掛ける
		forTargetVector = playerToBomb.Normalize() * kSpeedValue;
	};
private:
	//プレイヤーに向かっていく速さの倍数
	float kSpeedValue = 0.5f;
	//プレイヤー方向へ向かうベクトル
	Vector3 forTargetVector;
	//投げたかどうかのフラグ
	bool isThrowFlag = false;
	//動く範囲
	Vector3 boxMax;
	Vector3 boxMin;
#pragma region
	//ボスの弱点の当たり判定
	void ColliderInit();
	void OnCollision(const ICollider* colliderA);
	OBBoxCollider collider;
#pragma endregion Collider

};
