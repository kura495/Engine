#pragma once
#include "../IBossState.h"
class EAttackSlam : public IBossState
{
public:
	EAttackSlam() { stateType = BossState::AttackSlam; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	void OnCollisionAttack(Boss* boss,const ICollider& collider)override;
	std::string ShowState()override;
private:
	//AttackSlamPlayer
	bool IsAttackFlag = false;

	//叩きつけを一回以上しているか
	bool isSlam2ndFlag = false;
	//SEハンドル
	int SEHitattack;

};