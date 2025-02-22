#pragma once
#include "../IBossState.h"
class EAttackRocketPunch : public IBossState
{
public:
	EAttackRocketPunch() { stateType = BossState::RocketPunth; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
	void OnCollisionAttack(Boss* boss, const ICollider& collider)override;
private:
	enum mode {
		Preparation = 0,//準備
		Attack = 1,//攻撃
		Stay = 2,//隙
	};
	//ボスの待機関数
	void PreparationFunc(Boss* boss);
	//ボスの攻撃関数
	void AttackFunc(Boss* boss);

	//ターゲットに向かうベクトル
	Math::Vector3 forTargetVector = {0.0f,0.0f,0.0f};
	//もともとの位置
	Math::Vector3 PrePos = { 0.0f,0.0f,0.0f };;
	//プレイヤーへのベクトル
	Math::Vector3 playerToEnemy = { 0.0f,0.0f,0.0f };;
	//ランダムに動かす値
	const float ramdomvalue = 0.1f;
	//準備
	uint8_t modeFlag = mode::Preparation;
	//AttackModeの速さの倍率
	float speedValue_ = 0.1f;
	//AttackModeの速さの倍率
	float addSpeedValue_ = 0.05f;
};
