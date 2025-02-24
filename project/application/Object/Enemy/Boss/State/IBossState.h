#pragma once
#include <string>
#include "Vector/Vector3.h"
#include "Quaternion/Quaternion.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Utility/random/random.h"
enum AttackState {
	Slam,//叩きつけ
	Throw,//物を投げる
	RocketPunch,//ロケットパンチ！
	SwordSlash,//斬る攻撃
	END,//最後
};
//前方宣言
class Boss;
class ICollider;
enum class BossState {
	Root,
	ReturnPosition,//元の位置に戻る
	AttackSlam,//叩きつけ攻撃
	AttackThrowball,//爆弾を投げる攻撃
	RocketPunth,//ロケットパンチをする攻撃
	BeamSword,//ビーム剣攻撃
	Spawn,//出現時
	Dead,//死亡時
	Down,//ダウン時
	End,
};
class IBossState
{
public:
	IBossState() { stateType = BossState::End; };
	virtual ~IBossState() {};

	virtual void Init(Boss* p) = 0;
	virtual void Update(Boss* p) = 0;
	virtual void Draw(Boss* p) = 0;

	virtual void OnCollision(Boss* boss, const ICollider& collider) { boss; collider; };
	virtual void OnCollisionAttack(Boss* boss, const ICollider& collider) { boss; collider; };

	BossState GetStateType() { return stateType; };
	virtual std::string ShowState() { return "default"; };

protected:
	BossState stateType;

	//攻撃の選択をする
	static uint32_t isAttackSelect;
	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float addEaseT = 0.05f;
};