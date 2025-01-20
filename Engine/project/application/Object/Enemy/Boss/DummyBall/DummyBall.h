#pragma once
/*白いボール*/
#include "Object/IObject.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include <algorithm>
#include "ParticleSystem/ParticleSystem.h"

class DummyBall : public IObject
{
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	/// <summary>
	/// 爆弾を投げる時に呼び出す関数
	/// </summary>
	/// <param name="targetVector">ターゲットのワールド座標</param>
	void ThrowBall(Vector3 StertPoint, Vector3 TargetPoint, Vector3 targetVector) {
		isThrowFlag = true;
		startPoint = StertPoint;
		targetPoint = TargetPoint;
		isCompleteMoveFlag = false;
		world_.transform.translate = StertPoint;
		//方向を決める
		Vector3 playerToBomb = targetVector - TargetPoint;
		//速さの定数を掛ける
		forTargetVector = playerToBomb.Normalize() * kSpeedValue * accelValue;
		//コライダーを有効化
		collider.IsUsing = true;
	};
	/// <summary>
	/// ボールを飛ばす前の状態に戻す
	/// </summary>
	void Reset() {
		collider.IsUsing = false;
		isThrowFlag = false;
		isCompleteMoveFlag = false;
	}

	bool GetIsOverline() { return IsOverline; }
private:
	//プレイヤーに向かっていく速さの倍数
	float kSpeedValue = 0.2f;
	//プレイヤーが叩くたびに増える加速値
	float accelValue = 1.5f;
	//プレイヤー方向へ向かうベクトル
	Vector3 forTargetVector;
	//投げたかどうかのフラグ
	bool isThrowFlag = false;
	//移動が完了したかどうかのフラグ
	bool isCompleteMoveFlag = false;

#pragma region
	//ボスの弱点の当たり判定
	void ColliderInit();
	void OnCollision(const ICollider& colliderA);
	OBBoxCollider collider;
#pragma endregion Collider

#pragma region
	//パーティクル
	std::unique_ptr<ParticleSystem> particle_;
	Emitter emitter;
	void UpdateParticle(Particle& particle);
	Particle CustomParticle();
	const float kDeltaTime = 1.0f / 60.0f;
#pragma region パーティクル

	bool IsOverline = false;
	//TODO:デバッグ用
	float distance;
	void ImGui();

	bool isHit = false;

	Vector3 stertPos;
	Vector3 PrePos;

	Vector3 startPoint;
	Vector3 targetPoint;

	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float addEaseT = 0.1f;

};