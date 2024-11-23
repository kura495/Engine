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
	void ThrowBomb(Vector3 StertPos,Vector3 targetVector) {
		isThrowFlag = true;
		stertPos = StertPos;
		world_.transform.translate = stertPos;
		//方向を決める
		Vector3 playerToBomb = targetVector - world_.transform.translate;
		//速さの定数を掛ける
		forTargetVector = playerToBomb.Normalize() * kSpeedValue;
	};
	
	void Reset(Vector3 targetVector) {
		//方向を決める
		Vector3 playerToBomb = targetVector - world_.transform.translate;
		//速さの定数を掛ける
		forTargetVector = playerToBomb.Normalize() * kSpeedValue;
	}

	bool GetIsHit() { return isHit; }
private:
	//プレイヤーに向かっていく速さの倍数
	float kSpeedValue = 0.3f;
	//プレイヤー方向へ向かうベクトル
	Vector3 forTargetVector;
	//投げたかどうかのフラグ
	bool isThrowFlag = false;

#pragma region
	//ボスの弱点の当たり判定
	void ColliderInit();
	void OnCollision(const ICollider& colliderA);
	OBBoxCollider collider;
#pragma endregion Collider

	bool IsOverline = false;
	//TODO:デバッグ用
	float distance;
	void ImGui();

	bool isHit = false;

	Vector3 stertPos;

	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float addEaseT = 0.01f;


};
